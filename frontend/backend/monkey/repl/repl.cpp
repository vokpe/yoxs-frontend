#include "repl.hpp"

const std::string PROMPT = ">> ";

void REPL::tokenStart(std::istream& in, std::ostream& out){
    std::string line;

    while (true) {
        out << PROMPT;
        if (!std::getline(in, line)) {
            return; // Exit if there's an error or EOF is encountered
        }

        Lexer l(line);
        for (Token tok = l.NextToken(); tok.Type != TokenType::EOF_TOKEN; tok = l.NextToken()) {
            out << TokenTypeToString(tok.Type) << ": " << tok.Literal << "\n";
        }
    }
}

void REPL::parserStart(std::istream& in, std::ostream& out){
    std::string line;

    while (true) {
        out << PROMPT;
        if (!std::getline(in, line)) {
            return; // Exit if there's an error or EOF is encountered
        }

        Lexer l(line);
        Parser p(l);
        
        auto program = p.ParseProgram();
        if (!p.Errors().empty()) {
            printParserErrors(out, p.Errors());
            continue;
        }

        out << program->String();
        out << "\n";
    }
}   

void REPL::Start(std::istream& in, std::ostream& out) {
    std::string line;

    while (true) {
        out << PROMPT;
        if (!std::getline(in, line)) {
            return; // Exit if there's an error or EOF is encountered
        }

        Lexer l(line);
        Parser p(l);
        
        auto program = p.ParseProgram();
        if (!p.Errors().empty()) {
            printParserErrors(out, p.Errors());
            continue;
        }

        auto env = std::make_shared<Environment>();
        Evaluator evaluator;
        auto evaluated = evaluator.Eval(program, env);
        if(evaluated) {
            out << evaluated->Inspect() << "\n";
        }
    }
}

void REPL::StartSingle(std::istream& in, std::ostream& out) {
    std::string line;

    out << PROMPT;
    if (!std::getline(in, line)) {
        return; // Exit if there's an error or EOF is encountered
    }

    out << "Input: " << line << "\n";

    // Lexical Analysis
    out << "Starting Lexical Analysis...\n";
    Lexer lPrint(line);
    out << "Tokens:\n";
    for (Token tok = lPrint.NextToken(); tok.Type != TokenType::EOF_TOKEN; tok = lPrint.NextToken()) {
        out << "  " << TokenTypeToString(tok.Type) << ": '" << tok.Literal << "'\n";
        // Add more details here if needed, like line and character position
    }

    // Parsing
    out << "\nStarting Parsing...\n";
    Lexer l(line);
    Parser p(l);
    auto program = p.ParseProgram();
    if (!p.Errors().empty()) {
        printParserErrors(out, p.Errors());
        return; // Stop further processing if there are parsing errors
    }
    out << "Parsed Program (AST):\n  " << program->String() << "\n";

    // Evaluation
    out << "\nStarting Evaluation...\n";
    auto env = std::make_shared<Environment>();
    Evaluator evaluator;
    auto evaluated = evaluator.Eval(program, env);

    // Displaying the environment state could be added here

    if(evaluated) {
        out << "Evaluated Result: " << evaluated->Inspect() << "\n";
    } else {
        out << "No output from evaluation.\n";
    }

}


void REPL::printParserErrors(std::ostream& out, const std::vector<std::string>& errors) {
    out << "Woops! We ran into an error:\n";
    for (const auto& msg : errors) {
        out << "\t" << msg << "\n";
    }
}
