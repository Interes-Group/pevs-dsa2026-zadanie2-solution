import {test, expect, Shell} from "@microsoft/tui-test";
import * as path from "node:path";

const binDir = process.env.BIN_DIR || "../bin";
const vindex = path.join(binDir, "vindex");

test.use({shell: Shell.Bash, rows: 50});

test("should print help", async ({terminal}) => {
	terminal.submit(vindex + " --help");
	const helpTestStrings = ["help", "Help", "HELP", "Pomoc", "pomoc", "POMOC"];
	let passed = false;
	for (const testString of helpTestStrings) {
		try {
			await expect(terminal.getByText(testString, {strict: false, full: true})).toBeVisible({timeout: 500});
			passed = true;
			break;
		} catch (e) {
			passed = false;
		}
	}
	await expect(terminal).toMatchSnapshot();
	if (!passed) throw new Error("Help text not found");
});