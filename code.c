class Solution {
public:
	// Funktion zur Zählung der Anzahl von 3x3 magischen Quadraten im gegebenen Gitter
	int numMagicSquaresInside(vector<vector<int>>& grid) {
		int count = 0;  // Initialisieren des Zählers für magische Quadrate
		int m = grid.size();  // Anzahl der Zeilen im Gitter abrufen
		int n = grid[0].size();  // Anzahl der Spalten im Gitter abrufen
		
		// Durchlaufen aller möglichen 3x3-Submatrizen im Gitter
		for (int i = 0; i < m - 2; i++) {  // Durchlaufen der Zeilen bis m-2, um die Grenzen nicht zu überschreiten
			for (int j = 0; j < n - 2; j++) {  // Durchlaufen der Spalten bis n-2, um die Grenzen nicht zu überschreiten
				// Überprüfen, ob die 3x3-Submatrix mit der oberen linken Ecke bei (i, j) ein magisches Quadrat ist
				if (isMagicSquare(grid, i, j)) {
					count++;  // Wenn ja, den Zähler für magische Quadrate erhöhen
				}
			}
		}
		
		return count;  // Rückgabe der Gesamtzahl der gefundenen magischen Quadrate
	}

private:
	// Funktion zur Überprüfung, ob die 3x3-Submatrix, beginnend bei (i, j), ein magisches Quadrat ist
	bool isMagicSquare(vector<vector<int>>& grid, int i, int j) {
		// Initialisieren eines Vektors 'nums' der Größe 9, alle Elemente auf 0 gesetzt.
		// Dieser Vektor dient dazu, zu verfolgen, welche Zahlen von 1 bis 9 bereits in der Submatrix vorkamen.
		vector<int> nums(9, 0);

		// Durchlaufen der 3x3-Submatrix
		for (int x = i; x < i + 3; x++) {  // Durchlaufen der Zeilen der Submatrix
			for (int y = j; y < j + 3; y++) {  // Durchlaufen der Spalten der Submatrix
				// Überprüfen, ob das aktuelle Element gültig ist (im Bereich von 1 bis 9) und ob es nicht bereits vorkam (überprüft durch den Vektor nums)
				if (grid[x][y] < 1 || grid[x][y] > 9 || nums[grid[x][y] - 1] == 1) {
					// Wenn das Element ungültig ist oder bereits vorkam, wird false zurückgegeben, da es sich nicht um ein magisches Quadrat handelt.
					return false;
				}

				// Markieren des aktuellen Elements als gesehen, indem das entsprechende Element im 'nums'-Vektor auf 1 gesetzt wird
				nums[grid[x][y] - 1] = 1;
			}
		}

		// Berechnen der Zielsumme für die Zeilen, Spalten und Diagonalen
		// durch Summieren der ersten drei Elemente in der ersten Zeile
		int target = grid[i][j] + grid[i][j + 1] + grid[i][j + 2];
		
		// Überprüfen der Zeilen
		for (int x = i; x < i + 3; x++) {  // Durchlaufen der Zeilen der Submatrix
			int sum = 0;  // Initialisieren der Summe für die aktuelle Zeile
			for (int y = j; y < j + 3; y++) {  // Durchlaufen jedes Elements der Zeile
				sum += grid[x][y];  // Summieren der Elemente der Zeile
			}
			if (sum != target) {  // Wenn die Summe der Zeile nicht der Zielsumme entspricht, false zurückgeben
				return false;
			}
		}
		
		// Überprüfen der Spalten
		for (int y = j; y < j + 3; y++) {  // Durchlaufen der Spalten der Submatrix
			int sum = 0;  // Initialisieren der Summe für die aktuelle Spalte
			for (int x = i; x < i + 3; x++) {  // Durchlaufen jedes Elements der Spalte
				sum += grid[x][y];  // Summieren der Elemente der Spalte
			}
			if (sum != target) {  // Wenn die Summe der Spalte nicht der Zielsumme entspricht, false zurückgeben
				return false;
			}
		}
		
		// Überprüfen der Diagonalen
		int sum1 = 0, sum2 = 0;
		for (int k = 0; k < 3; k++) {
			// Summieren der Elemente der ersten Diagonale (von oben links nach unten rechts)
			sum1 += grid[i + k][j + k];
			// Summieren der Elemente der zweiten Diagonale (von oben rechts nach unten links)
			sum2 += grid[i + k][j + 2 - k];
		}
		// Wenn eine der Diagonalsummen nicht der Zielsumme entspricht, false zurückgeben
		if (sum1 != target || sum2 != target) {
			return false;
		}
		
		// Wenn alle Überprüfungen bestanden sind, handelt es sich um ein magisches Quadrat
		return true;
	}
};
