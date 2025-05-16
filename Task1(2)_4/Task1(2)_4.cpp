#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

bool Operator(char op, bool left, bool right) {
	if (op == '&') return left && right;
	if (op == '|') return left || right;
	return false;
}

bool Variables(char var, bool K, bool M, bool A) {
	if (var == 'K') return K;
	if (var == 'M') return M;
	if (var == 'A') return A;
	return false;
}

bool statement(const string& expression, bool K, bool M, bool A) {
	vector<bool> values;
	vector<char> operators;

	for (int i = 0; i < expression.size(); ++i) {
		char ch = expression[i];

		if (ch == ' ') continue;

		if (ch == 'K' || ch == 'M' || ch == 'A') {
			values.push_back(Variables(ch, K, M, A));
		}
		else if (ch == '!') {
			if (i + 1 < expression.size()) {
				++i;
				bool val = Variables(expression[i], K, M, A);
				values.push_back(!val);
			}
			else {
				cerr << "Error: '!' operator not followed by a variable.\n";
				return false;
			}
		}
		else if (ch == '&' || ch == '|') {
			operators.push_back(ch);
		}
		else {
			cout << "undefined variable or operator\n";
		}
	}

	while (!operators.empty()) {
		char op = operators.front();
		operators.erase(operators.begin());

		bool left = values.front();
		values.erase(values.begin());
		bool right = values.front();
		values.erase(values.begin());

		values.insert(values.begin(), Operator(op, left, right));
	}

	return values.empty() ? false : values.front();
}

int main() {
	// Truth table inputs (for 3 variables as an example)
	bool K[] = { false, false, false, false, true, true, true, true };
	bool M[] = { false, false, true, true, false, false, true, true };
	bool A[] = { false, true, false, true, false, true, false, true };

	// Number of rows in the truth table
	const int rows = 8;

	// Number of premises
	int numPremises;
	cout << "Enter the number of premises: ";
	cin >> numPremises;
	cin.ignore();

	vector<string> premises(numPremises);
	for (int i = 0; i < numPremises; ++i) {
		cout << "Enter premise " << i + 1 << " (use variables K, M, A and logical operators like !, |, &): ";
		getline(cin, premises[i]);
	}

	// Input for conclusion
	string conclusion;
	cout << "Enter the conclusion (use variables K, M, A and logical operators like !, |, &): ";
	getline(cin, conclusion);

	// Table header
	cout << "K" << setw(10) << "M" << setw(10) << "A";
	for (int i = 0; i < numPremises; ++i) {
		cout << setw(15) << "Premise" << i + 1;
	}
	cout << setw(15) << "Conclusion";
	cout << "\n";
	cout << string(100, '-') << "\n"; // Dynamic separator

	// Variables for satisfiability and validity
	bool satisfiable = false;
	bool valid = true;

	// Evaluate the truth table
	for (int i = 0; i < rows; ++i) {
		cout << K[i] << setw(10) << M[i] << setw(10) << A[i];

		bool allPremisesTrue = true;
		for (int j = 0; j < numPremises; ++j) {
			bool premiseValue = statement(premises[j], K[i], M[i], A[i]);
			cout << setw(15) << premiseValue;
			if (!premiseValue) {
				allPremisesTrue = false;
			}
		}

		bool conclusionValue = statement(conclusion, K[i], M[i], A[i]);
		cout << setw(15) << conclusionValue;

		if (allPremisesTrue) {
			satisfiable = true;
			if (!conclusionValue) {
				valid = false;
			}
		}

		cout << "\n";
	}

	// Output satisfiability 
	if (satisfiable) { cout << "\nSet of Premises is Satisfiable.\n"; 
	//Output for validity
	if (valid) cout << "Argument is Valid. \n";
	else cout << "Argument is Falsifiable.\n";

	}
	else cout << "Set of Premises is Not Satisfiable.\n";

	
	return 0;
}