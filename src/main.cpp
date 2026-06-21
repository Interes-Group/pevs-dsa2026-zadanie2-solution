#include <iostream>
#include <random>
#include <fstream>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <cctype>
#include <iomanip>

using namespace std;

namespace trie {
    // Stores per-file occurrence count of a word
    struct FileEntry {
        string fileName;
        int frequency;

        FileEntry(string name) : fileName(std::move(name)), frequency(1) {
        }
    };

    class Node {
    private:
        map<char, Node *> children;
        bool endOfWord;
        // Filled only when isEndOfWord == true
        vector<FileEntry> files; // one entry per file
        int totalFrequency; // sum across all files

        // Recursively deletes all nodes in the subtree rooted at 'node'
        void clearNode(Node *node) {
            for (auto &pair: node->children) {
                clearNode(pair.second);
                delete pair.second;
            }
            node->children.clear();
            node->endOfWord = false;
            node->files.clear();
            node->totalFrequency = 0;
        }

    public:
        Node() : endOfWord(false), totalFrequency(0) {
        }

        ~Node() {
            clearNode(this);
        }

        [[nodiscard]] Node *getChild(char ch) const {
            return children.find(ch) == children.end() ? nullptr : children.at(ch);
        }

        void addChild(char ch, Node *node) {
            children[ch] = node;
        }

        [[nodiscard]] bool hasChildren() const {
            return !children.empty();
        }

        Node *removeChild(char ch) {
            auto it = children.find(ch);
            if (it == children.end()) return nullptr;
            Node *node = it->second;
            children.erase(it);
            return node;
        }

        [[nodiscard]] bool isEndOfWord() const { return endOfWord; }
        void isEndOfWord(bool value) { endOfWord = value; }
        [[nodiscard]] int getTotalFrequency() const { return totalFrequency; }
        [[nodiscard]] const vector<FileEntry> &getFiles() const { return files; }

        void addFile(const string &fileName) {
            if (!this->endOfWord) return;
            this->totalFrequency++;
            for (auto &fileEntry: files) {
                if (fileEntry.fileName == fileName) {
                    fileEntry.frequency++;
                    return;
                }
            }
            files.emplace_back(fileName);
        }

        void clearFiles() {
            files.clear();
            totalFrequency = 0;
        }

        void clear() {
            clearNode(this);
        }
    };

    class Trie {
    private:
        Node *root;
        set<string> files;

        // Returns true if the node should be deleted by its parent (has no children and is not an end of another word)
        bool removeWord(Node *current, const std::string &word, int depth) {
            if (depth == static_cast<int>(word.size())) {
                if (!current->isEndOfWord()) {
                    return false; // word doesn't exist
                }
                current->isEndOfWord(false);
                current->clearFiles();
                return !current->hasChildren(); // safe to delete if no children
            }

            char ch = word[depth];
            auto it = current->getChild(ch);
            if (it == nullptr) {
                return false; // word doesn't exist
            }

            bool shouldDeleteChild = removeWord(it, word, depth + 1);

            if (shouldDeleteChild) {
                delete it;
                current->removeChild(ch);
                // This node can also be deleted if it's not an end and has no other children
                return !current->hasChildren() && !current->isEndOfWord();
            }

            return false;
        }

    public:
        Trie() {
            root = new Node();
        }

        ~Trie() {
            delete root;
            files.clear();
        }

        [[nodiscard]] Node *getRoot() const {
            return root;
        }

        [[nodiscard]] const set<string> &getFiles() const {
            return files;
        }

        void addFile(const string &fileName) {
            files.insert(fileName);
        }

        // Inserts a word and records the file it came from
        void insert(const string &word, const string &fileName) {
            Node *current = root;

            for (char ch: word) {
                if (current->getChild(ch) == nullptr) {
                    current->addChild(ch, new Node());
                }
                current = current->getChild(ch);
            }

            current->isEndOfWord(true);
            current->addFile(fileName);
        }

        // Returns the end node of the word, or nullptr if not found
        [[nodiscard]] Node *search(const string &word) const {
            Node *current = root;

            for (char ch: word) {
                if (current->getChild(ch) == nullptr) {
                    return nullptr;
                }
                current = current->getChild(ch);
            }

            return current->isEndOfWord() ? current : nullptr;
        }

        // Removes a single word from the trie.
        // Returns true if the word was found and removed, false if it didn't exist.
        bool remove(const std::string &word) {
            if (search(word) == nullptr) {
                return false;
            }
            removeWord(root, word, 0);
            return true;
        }

        // Removes all words from the trie, leaving it empty.
        void clear() {
            root->clear();
            files.clear();
        }
    };
} // namespace trie

const string INDEX_FILE = "vindex_index.txt";

struct TrieStats {
    int uniqueWords = 0;
    int totalOccurrences = 0;
    long long totalWordLen = 0; // for average length calculation
    set<string> files; // unique file names across all words

    // word -> totalFrequency, for top-5
    vector<pair<string, int> > wordFrequencies;

    // word -> number of files it appears in, for "most spread" word
    pair<string, int> mostSpreadWord = {"", 0};
};

string readFile(const string &filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file: " << filePath << endl;
        return {};
    }
    string content((istreambuf_iterator<char>(file)),
                   istreambuf_iterator<char>());
    file.close();
    return content;
}

vector<string> preProcessWords(const string &content) {
    istringstream stream(content);
    vector<string> result;
    string word;

    while (stream >> word) {
        // Convert to lowercase
        transform(word.begin(), word.end(), word.begin(),
                  [](unsigned char c) { return tolower(c); });

        // Remove all non-alphanumeric characters
        word.erase(remove_if(word.begin(), word.end(),
                             [](unsigned char c) { return !isalnum(c); }),
                   word.end());

        // Skip empty strings
        if (!word.empty()) {
            result.push_back(word);
        }
    }

    return result;
}

// Recursively walks the trie and fills in the TrieStats struct.
// 'currentWord' is built up character by character as we descend.
void collectStats(trie::Node *node, const string &currentWord, TrieStats &stats) {
    if (node == nullptr) return;

    if (node->isEndOfWord()) {
        stats.uniqueWords++;
        stats.totalOccurrences += node->getTotalFrequency();
        stats.totalWordLen += static_cast<long long>(currentWord.size());
        stats.wordFrequencies.emplace_back(currentWord, node->getTotalFrequency());

        int fileCount = static_cast<int>(node->getFiles().size());
        if (fileCount > stats.mostSpreadWord.second) {
            stats.mostSpreadWord = {currentWord, fileCount};
        }

        for (const auto &entry: node->getFiles()) {
            stats.files.insert(entry.fileName);
        }
    }

    for (char ch = 'a'; ch <= 'z'; ch++) {
        trie::Node *child = node->getChild(ch);
        if (child != nullptr) {
            collectStats(child, currentWord + ch, stats);
        }
    }
    // also check digits 0-9
    for (char ch = '0'; ch <= '9'; ch++) {
        trie::Node *child = node->getChild(ch);
        if (child != nullptr) {
            collectStats(child, currentWord + ch, stats);
        }
    }
}

// Recursively walks the trie and writes every end-of-word node as one line.
void saveNode(trie::Node *node, const string &currentWord, ofstream &out) {
    if (node == nullptr) return;

    if (node->isEndOfWord()) {
        out << currentWord;
        const auto &files = node->getFiles();
        for (int i = 0; i < static_cast<int>(files.size()); i++) {
            out << (i == 0 ? " " : ",");
            out << files[i].fileName << ":" << files[i].frequency;
        }
        out << "\n";
    }

    for (char ch = 'a'; ch <= 'z'; ch++) {
        trie::Node *child = node->getChild(ch);
        if (child != nullptr) {
            saveNode(child, currentWord + ch, out);
        }
    }
    for (char ch = '0'; ch <= '9'; ch++) {
        trie::Node *child = node->getChild(ch);
        if (child != nullptr) {
            saveNode(child, currentWord + ch, out);
        }
    }
}

// Saves the entire trie to INDEX_FILE.
// Returns true on success, false if the file could not be opened.
bool saveTrie(trie::Trie &t) {
    ofstream out(INDEX_FILE);
    if (!out.is_open()) {
        cerr << "Error: Could not open index file for writing: " << INDEX_FILE << endl;
        return false;
    }
    saveNode(t.getRoot(), "", out);
    out.close();
    return true;
}

// Loads the trie from INDEX_FILE.
// If the file does not exist it is created empty — that is not an error.
// Returns true on success, false on a read error.
bool loadTrie(trie::Trie &t) {
    // If file doesn't exist, create it empty and return
    ifstream check(INDEX_FILE);
    if (!check.is_open()) {
        ofstream create(INDEX_FILE);
        return true;
    }
    check.close();

    ifstream in(INDEX_FILE);
    if (!in.is_open()) {
        cerr << "Error: Could not open index file for reading: " << INDEX_FILE << endl;
        return false;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        // Split line into word and file-list parts
        istringstream lineStream(line);
        string word;
        string filePart;
        lineStream >> word >> filePart;

        if (word.empty() || filePart.empty()) continue;

        // Parse "filename1:count1,filename2:count2,..."
        istringstream fileStream(filePart);
        string token;
        while (getline(fileStream, token, ',')) {
            size_t colonPos = token.rfind(':');
            if (colonPos == string::npos) continue;

            string fileName = token.substr(0, colonPos);
            int frequency = stoi(token.substr(colonPos + 1));

            // Re-insert the word once per recorded occurrence in that file
            for (int i = 0; i < frequency; i++) {
                t.insert(word, fileName);
            }
        }
    }

    in.close();

    TrieStats stats;
    collectStats(t.getRoot(), "", stats);
    for (const string &file: stats.files) {
        t.addFile(file);
    }

    return true;
}

// Converts a raw argc/argv into a vector of strings, skipping argv[0] (program name)
vector<string> parseArgs(int argc, char *argv[]) {
    vector<string> args;
    for (int i = 1; i < argc; i++) {
        args.emplace_back(argv[i]);
    }
    return args;
}

// ── Command handlers ─────────────────────────────────────────────────────────

void cmdHelp() {
    cout << "VINDEX - Verbal Indexing Engine" << endl;
    cout << "Help - available commands:" << endl;
    cout << "  index <file>   Index a text file into the search index" << endl;
    cout << "  search <word>  Search for a word in the index" << endl;
    cout << "  stats          Show statistics about the index" << endl;
    cout << "  clear          Clear the entire index" << endl;
    cout << "  help           Show this help message" << endl;
}

void cmdIndex(const vector<string> &args, trie::Trie &t) {
    if (args.empty()) {
        cerr << "Error: 'index' command requires a file path argument." << endl;
        cerr << "Usage: vindex index <file>" << endl;
        return;
    }

    const string &filePath = args[0];
    string content = readFile(filePath);
    if (content.empty()) {
        return; // readFile already printed the error
    }

    if (t.getFiles().count(filePath) == 1) {
        cout << "File '" << filePath << "' was already indexed" << endl;
        return;
    }

    vector<string> words = preProcessWords(content);
    for (const string &word: words) {
        t.insert(word, filePath);
    }
    t.addFile(filePath);
    cout << "Indexed " << words.size() << " words from: " << filePath << endl;
}

void cmdSearch(const vector<string> &args, trie::Trie &t) {
    if (args.empty()) {
        cerr << "Error: 'search' command requires a word argument." << endl;
        cerr << "Usage: vindex search <word>" << endl;
        return;
    }

    const string &word = args[0];
    trie::Node *result = t.search(word);

    if (result == nullptr) {
        cout << "Word '" << word << "' was not found in the index." << endl;
        return;
    }

    cout << "Word: \"" << word << "\"" << endl;
    for (const auto &entry: result->getFiles()) {
        cout << "  " << entry.fileName << " : " << entry.frequency << " occurrence(s)" << endl;
    }
    cout << "  Total: " << result->getTotalFrequency() << " occurrence(s)" << endl;
}

void cmdStats(trie::Trie &t) {
    TrieStats stats;
    collectStats(t.getRoot(), "", stats);

    // ── 1. Unique words ───────────────────────────────────────────────────
    cout << "=== VINDEX Statistics ===" << endl;
    cout << "Unique indexed words   : " << stats.uniqueWords << endl;

    // ── 2. Files ──────────────────────────────────────────────────────────
    cout << "Processed files        : " << stats.files.size() << endl;
    for (const auto &f: stats.files) {
        cout << "  - " << f << endl;
    }

    // ── 3. Top 5 most frequent words ──────────────────────────────────────
    sort(stats.wordFrequencies.begin(), stats.wordFrequencies.end(),
         [](const pair<string, int> &a, const pair<string, int> &b) {
             return a.second > b.second;
         });

    cout << "Top 5 most frequent words:" << endl;
    int limit = min(5, static_cast<int>(stats.wordFrequencies.size()));
    for (int i = 0; i < limit; i++) {
        cout << "  " << (i + 1) << ". \"" << stats.wordFrequencies[i].first
                << "\" — " << stats.wordFrequencies[i].second << " occurrence(s)" << endl;
    }

    // ── 4. Extra stats ────────────────────────────────────────────────────
    cout << "Total word occurrences : " << stats.totalOccurrences << endl;

    if (stats.uniqueWords > 0) {
        double avgLen = static_cast<double>(stats.totalWordLen) / stats.uniqueWords;
        cout << "Average word length    : " << fixed << setprecision(2) << avgLen << " characters" << endl;
    }

    if (!stats.mostSpreadWord.first.empty()) {
        cout << "Most spread word       : \"" << stats.mostSpreadWord.first
                << "\" (appears in " << stats.mostSpreadWord.second << " file(s))" << endl;
    }
}

void cmdClear(trie::Trie &t) {
    t.clear();
    cout << "Index has been cleared successfully." << endl;
}

// ── Argument dispatcher ───────────────────────────────────────────────────────

void processArgs(const vector<string> &args, trie::Trie &t) {
    if (args.empty()) {
        cerr << "Error: No command provided." << endl;
        cmdHelp();
        return;
    }

    const string &command = args[0];
    // Command arguments are everything after the command itself
    vector<string> cmdArgs(args.begin() + 1, args.end());

    if (command == "help" || command == "--help") {
        cmdHelp();
    } else if (command == "index") {
        cmdIndex(cmdArgs, t);
    } else if (command == "search") {
        cmdSearch(cmdArgs, t);
    } else if (command == "stats") {
        cmdStats(t);
    } else if (command == "clear") {
        cmdClear(t);
    } else {
        cerr << "Error: Unknown command '" << command << "'." << endl;
        cmdHelp();
    }
}

int main(int argc, char *argv[]) {
    trie::Trie t;

    loadTrie(t);

    vector<string> args = parseArgs(argc, argv);
    processArgs(args, t);

    saveTrie(t);

    return 0;
}
