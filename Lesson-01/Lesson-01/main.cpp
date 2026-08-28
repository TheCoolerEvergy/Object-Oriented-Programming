#include <iostream>;
using namespace std;

#include <random>;
std::default_random_engine generator;
std::uniform_int_distribution<int> damageDist(1, 3); // damage range
int dmg_roll_bonus = damageDist(generator); // random multiplier for base damage

int main()
{
	// INITIALIZING VARIABLES
	int enemyHealth = 100;
	int enemyAttackDamage = 4;
	int choice = 0;
	int roundNum = 0;
	string restartChoice = "y";

	// player class
	class Player
	{
		public:
			int maxHealth = 100;
			int maxMana = 100;
			int manaRegen = 5;
			int attackDamage = 5;
};

	void gameLoop();
	{
		int health = Player::maxHealth;
		int mana = Player::maxMana;
		int manareg = Player:manaRegen;
		int bd = Player:attackDamage;

		while (health > 0 && enemyHealth > 0)
		{
			// --- PLAYER CHOICE LOOP ---
			roundNum += 1;
			cout << "\nRound: " << roundNum << endl;
			cout << "Player Health: " << player(health) << endl;
			cout << "Player Mana: " << player(mana) << endl;
			cout << "Enemy Health: " << enemyHealth << endl;
			cout << "\n1. Attack\n2. Defend\n3. Heal (Costs 15 Mana)\nChoose: "; // asks for the player's choice
			cin >> choice; // collects the player's choice

			if (choice == 1) 
			{
				int dam = 0; // temp damage variable
				dam = bd * dmg_roll_bonus; // uses the rolled damage multiplier to increase damage (and make it random)

				enemyHealth -= d;

				cout << "You attacked the enemy for " << 
			}

			if (enemyHealth > 0)
			{
				int dam = 0; // temp damage variable
				dam = enemyAttackDamage * dmg_roll_bonus; // uses the rolled damage multiplier to increase damage (and make it random)
				int enemyDamage = (choice == 2) ? dam-4 : dam; // reduces enemy damage by 4 if the player defends
				player(health) -= enemyDamage;
				cout << "The enemy attacks you for " << enemyDamage << " damage!\n";
			}



			cout << ((player(health) <= 0) ? "\nYou were defeated!\n" : "\nEnemy defeated!\n");
			cout << "The battle lasted for " << roundNum << " rounds!\n";

			// --- RESTART LOGIC ---
			cout << "\n\n--------------------\n";
			cout << "Would you like to play again?\nY/N?\nChoose: ";
			cin >> restartChoice;

			if (restartChoice == "y" || restartChoice == "Y") { gameLoop(); }
			else if (restartChoice == "n" || restartChoice == "N") { return 0; }

			while (restartChoice != "y" && restartChoice != "Y" && restartChoice != "n" && restartChoice != "N") {
				cout << "That answer is invalid.";
				cout << "Would you like to play again?\nY/N?\nChoose: ";
				cin >> restartChoice;

				if (restartChoice == "y" || restartChoice == "Y") { gameLoop(); }
				else if (restartChoice == "n" || restartChoice == "N") { return 0; }
			}
			return 0;
		}
	}

	gameLoop(); // runs the game loop
}