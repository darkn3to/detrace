<h1 align="center">detrace - AST-Based Similarity Analysis for C Code</h1>

Detrace is a static analysis tool for measuring code similarity in C programs. It is designed for post-submission evaluation, particularly in environments where time constraints limit code variation. The tool identifies overlap by analyzing the abstract syntax tree of each submission. It outputs a similarity score reflecting the degree of structural resemblance between programs.

## Features
• <b>AST-based Comparison</b> for analyzing structural similarity between C programs.

• <b>Code Abstraction</b> through identifier removal, empty construct elimination, and function reordering tolerance.

• <b>Similarity Scoring</b> using subtree-level comparison to generate a percentage match.

• <b>One-to-many Comparison</b> to efficiently evaluate a single submission against multiple peers.

• <b>Multithreaded Execution</b> (experimental) for parallel similarity computation.

## Working Overview
<b>Detrace</b> starts by tokenizing the input C code using a custom lexer implemented as a <b>finite state machine</b>.<br>
The token stream is parsed using a <b>YACC-based grammar</b> with rich semantic actions to build a <b>parse tree</b>.<br>
This tree is then simplified by abstracting away identifiers and function names, and by removing empty loops and blocks to generate the <b>Abstract Syntax Tree (AST)</b>.
Function reordering is also handled to normalize superficial differences in code layout.<br>
Each subtree in the AST is <b>hashed</b>, and the hashes are floated upward to form cumulative representations.
Subtrees are grouped by size, allowing structurally significant matches to be weighted more heavily.<br>
A <b>similarity score</b> is computed based on the number and size of matching subtrees between submissions.
The score reflects how structurally alike two programs are, rather than relying on textual overlap.
The process supports one-to-many comparisons, with multithreading integrated for experimental scalability.

## Usage
1. Clone repository and open directory.
2. Run `make` command.
3. Run the command:
   ```cmd
   ./detrace <source_file> <target_file_1> <target_file_2> ... <target_file_n> --m (for multithreading; optional)
   ```
