lithium
=======

Lithium is a text editor written with gtkmm in C++.

## Features

These are some features I hope to implement.

### Network file serving (concurrent editing)
  + Network discovery (other servers)

### libgit2: git integration
  + commit from editor, view commit history
  + commit lines and partail changes (from last commit to current state) as needed.
  + simple gutter diff.

### Integration with v8 (JS vm)
  + Scriptable elements of the global "Lithium" object
  + scriptability of command palette, etc.
  + git API exposed to js

### simple style & syntax specification
  + gtk supports css, allow for syntax.
  + JSON files to specify regex-based syntax (css-able ids for each type)
  + CSS can style named ids
