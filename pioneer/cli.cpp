#include "cli.h"

using namespace std;

static bool compareCaseInsensitive(const string& a, const string& b)
{
	string lowerStr1 = a;
	string lowerStr2 = b;

	transform(lowerStr1.begin(), lowerStr1.end(), lowerStr1.begin(),
		[](unsigned char c) { return tolower(c); });
	transform(lowerStr2.begin(), lowerStr2.end(), lowerStr2.begin(),
		[](unsigned char c) { return tolower(c); });

	return lowerStr1 == lowerStr2;
}

void start_cli()
{
	Book book;

	while (true) 
	{
		string input;

		cout << ">> ";
		getline(cin, input);

		vector<string> _split = split(input, " ");
		if (compareCaseInsensitive(_split[0], "make"))
		{
			if (_split.size() < 3)
			{
				cout << "Usage: make <pgn_file_name> <out_file_name>" << endl;
				continue;
			}

			string pgn_file_name = _split[1];
			string out_file_name = _split[2];

			ofstream out_file(out_file_name, ios::app);
			ifstream pgn_file(pgn_file_name);

			if (!pgn_file.is_open())
			{
				cout << "Error opening file " << pgn_file_name << "." << endl;
				continue;
			}

			stringstream stream;
			stream << pgn_file.rdbuf();

			string raw_pgns = stream.str();
			vector<string> pgns = splitPgns(raw_pgns);

			for (const auto& pgn : pgns)
			{
				Pgn _pgn(pgn);
				book.insertFromPgn(_pgn);
			}

			book.write_book(out_file);

			out_file.close();
			pgn_file.close();

			cout << "Successfully written the book 📝" << endl;

		}
		else if (compareCaseInsensitive(_split[0], "load"))
		{
			if (_split.size() < 2)
			{
				cout << "Usage: load <file_name>" << endl;
				continue;
			}

			string inp_file_name = _split[1];
			ifstream inp_file(inp_file_name);
			if (!inp_file.is_open())
			{
				cout << "Error opening file " << inp_file_name << "." << endl;
				continue;
			}

			book.read_book(inp_file);
			cout << "Book loaded successfully 📖" << endl;

		}
		else if (compareCaseInsensitive(_split[0], "getm"))
		{
			if (_split.size() < 2)
			{
				cout << "Usage: getm <FEN>" << endl;
				continue;
			}

			string fen = trim(input.substr(5));
			Board& board = Board::fromFen(fen);

			Move move = book.getRandMove(board);
			if (move.isNull()) 
			{
				cout << "Cannot find move :(" << endl;
				continue;
			}

			cout << move.toUci() << endl;

		}
		else if (compareCaseInsensitive(_split[0], "getrm"))
		{
			if (_split.size() < 3)
			{
				cout << "Usage: getrm <rank> <FEN>" << endl;
				continue;
			}

			char rank = stoi(_split[1]) & 0xFF;
			string fen = trim(input.substr(6 + _split[1].size()));

			Board board;
			board.fromFen(fen);

			Move move = book.getRankedMove(board, rank);
			if (move.isNull())
			{
				cout << "Cannot find move :(" << endl;
				continue;
			}

			cout << move.toUci() << endl;
		}
		else if (compareCaseInsensitive(_split[0], "help")) 
		{
			cout << "Usage: make <pgn_file_name> <out_file_name>" << endl;
			cout << "Usage: load <file_name>" << endl;
			cout << "Usage: getrm <rank> <FEN>" << endl;
			cout << "Usage: getm <FEN>" << endl;
			cout << "Usage: quit (quit's the command line interface)" << endl;

		}
		else if (compareCaseInsensitive(_split[0], "quit"))
		{
			return;
		}
		else 
		{
			cout << "Invalid command." << endl;
		}
	}
}
