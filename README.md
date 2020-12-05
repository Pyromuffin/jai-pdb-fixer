# jai-pdb-fixer
fixes up .pdb files produced bye the jai compiler so you can use natvis on arrays

just run the exe on the pdb file you're interested in fixing up.
I've placed pdb_fixer.exe in jai/bin/ and jai.natvis in jai/.vscode.

This is what my vscode launch.json looks like:

```
{
    "version": "0.2.0",
    "configurations": [
        {
            "preLaunchTask": "build",
            "type": "cppvsdbg",
            "request": "launch",
            "name": "Launch Program",
            "program": "${fileDirname}/${fileBasenameNoExtension}.exe",
            "cwd": "${workspaceRoot}",
            "visualizerFile": "${workspaceFolder}/.vscode/jai.natvis",
        },
    ]
  }
  ```
  
  and tasks.json:
  ```
  {
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build-exe",
            "type": "shell",
            "command": "c:/users/pyrom/desktop/jai/bin/jai.exe",
            "args": ["${file}"],
            "group": "build",
            "presentation": {
                "reveal": "always",
                "clear": true,
                "showReuseMessage": false
            },
            "problemMatcher": {
                "owner": "jai",
                "fileLocation": ["absolute"],
                "pattern": {
                  "regexp": "^[^:]*:(.*):(\\d+),(\\d+):\\s+(Warning|Error):\\s+(.*)$",
                  "file": 1,
                  "line": 2,
                  "column": 3,
                  "severity": 4,
                  "message": 5,
                  "code": 6
                }
            }
        },
        {
            "dependsOn": "build-exe",
            "label": "build",
            "type": "shell",
            "command": "c:/users/pyrom/desktop/jai/bin/pdb_fixer.exe",
            "args": ["${fileDirname}/${fileBasenameNoExtension}.pdb"],
            "group": "build",
            "presentation": {
                "reveal": "always",
                "clear": true,
                "showReuseMessage": false
            },
        }
    ]
}
```

This works for debugging pretty well!
Except to get breakpoints you'll need to add 
```
        "breakpoints": [{ "language": "jai" }],
```
to the contributes block of a package.json file for whatever jai extension you're using. I've added this to my extension at: https://github.com/Pyromuffin/jai-lsp
