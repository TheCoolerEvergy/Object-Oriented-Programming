#include <iostream>;
using namespace std;

int main()
{
	int playerHealth = 100;
	int playerMana = 100;
	int playerManaRegen = 5;
	int enemyHealth = 100;
	int choice = 0;
	int roundNum = 0;


	while (playerHealth > 0 && enemyHealth > 0)
	{
		// game output
		roundNum += 1;
		cout << "\nRound: " << roundNum << endl;
		cout << "Player Health: " << playerHealth << endl;
		cout << "Player Mana: " << playerMana << endl;
		cout << "Enemy Health: " << enemyHealth << endl;
		cout << "\n1. Attack\n2. Defend\n3. Heal (Costs 15 Mana)\nChoose: ";
		cin >> choice; // player input

		if (choice == 1)
		{
			enemyHealth -= 15;
			cout << "You attack the enemy for 15 damage!\n";
			playerMana += playerManaRegen;
		}
		else if (choice == 2)
		{
			playerMana += 20;
			cout << "You defend, reducing incoming damage and restore 20 Mana.\n";
		}
		else if (choice == 3)
		{
			playerMana -= 15;
			playerHealth += 25;
			if (playerHealth > 100) { playerHealth = 100; } // if player is healed beyond 100, their HP is reset to 100
			cout << "You cast a healing spell, restoring your Health by 25 and reducing your Mana by 15!";
			playerMana += playerManaRegen;
		}

		else if (playerMana < 15 && choice == 3) // skips player turn and ensures the player cannot cast the spell if their mana is too low
		{
			cout << "You do not have enough Mana to cast a healing spell!\n";
		}

		else if (choice < 1 || choice > 3) // skips player turn if they choose an option that is not on the list
		{
			cout << "That option is not on the list!\n";
		}

		if (enemyHealth > 0)
		{
			int enemyDamage = (choice == 2) ? 4 : 10; // reduces enemy damage by 4 if the player defends
			playerHealth -= enemyDamage;
			cout << "The enemy attacks you for " << enemyDamage << " damage!\n";
		}
	}

	cout << ((playerHealth <= 0) ? "\nYou were defeated!\n" : "\nEnemy defeated!\n");
	cout << "The battle lasted for " << roundNum << " rounds!\n";
		return 0;
}