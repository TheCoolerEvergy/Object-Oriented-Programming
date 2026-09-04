#include <iostream>;
using namespace std;

#include <random>;
std::default_random_engine generator;
std::uniform_int_distribution<int> enemyDamDist(1, 15); // enemy damage range
std::uniform_int_distribution<int> playerDamDist(1, 10); // player damage range
std::uniform_int_distribution<int> enemyChoiceRoll(1, 5); // enemy choice range

// INITIALIZING VARIABLES
// enemy variables
int enemyMaxHealth = 140;
int enemyAttackDamage = 12;
int enemyChoice = 0;
int enemyMaxMana = 50;
int enemyManaRegen = 5;

// player variables
int maxHealth = 200;
int maxMana = 50;
int manaRegen = 5;
int attackDamage = 15;
int choice = 0;
string restartChoice = "y";
int roundNum = 0;

// signals the game that it is running
bool gameRunning = true;

int main()
{
	// reset game values
	roundNum = 0;
	// reset player values
	int health = maxHealth;
	int mana = maxMana;
	int bd = attackDamage;
	int ppc = choice; // past player choice
	bool redoChoice = false;
	// reset enemy values
	int eHealth = enemyMaxHealth;
	int eMana = enemyMaxMana;
	int eMReg = enemyManaRegen;
	int ebd = enemyAttackDamage;
	int ec = enemyChoice;
	int ecw = 0;
	int pec = ec; // past enemy choice
	bool canBlock = false;

	while (gameRunning)
	{
		// round increments
		roundNum += 1;
		eMana += eMReg; // restores enemy mana
		if (eMana > enemyMaxMana) { eMana = enemyMaxMana; }
		mana += manaRegen; // restores player mana
		if (mana > maxMana) { mana = maxMana; }


		std::cout << "\n\n\nRound: " << roundNum << endl;

		// --- PLAYER CHOICE LOOP ---
		if (health > 0)
		{
			redoChoice = true;

			while (redoChoice == true)
			{
				std::cout << "Player Health: " << health << endl;
				std::cout << "Player Mana: " << mana << " (" << manaRegen << "/regen per round) " << endl;
				std::cout << "Enemy Health: " << eHealth << endl;
				std::cout << "\n1. Attack\n2. Defend\n3. Heal (Costs 20 Mana)\n4. Mana Blast (Costs 30 Mana)\n5. Recover Mana\nChoose: "; // asks for the player's choice
				std::cin >> choice; // collects the player's choice



				if (choice < 1 || choice > 5)
				{
					std::cout << "\nThis choice is invalid, please choose another.\n";

					redoChoice = true;
				}


				else if (choice == 1) // Action 1, Attack
				{
					std::cout << "\nYou Attacked the enemy with your weapon!\n";

					int bonus = playerDamDist(generator); // generates a damage multiplier for the attack's damage

					int dam = bd + bonus; // uses the rolled damage multiplier to increase damage (and make it random)

					if (ec == 2 && canBlock == true)
					{
						dam -= 8; // reduces attack damage by 8 if enemy Defended
					}
				

					eHealth -= dam; // hit damage
					std::cout << "You Attacked the enemy for " << dam << " damage!\n";

					redoChoice = false;
				}

	
				else if (choice == 2) // Action 2, Defend
				{
					std::cout << "\nYou Defended yourself!\n";
	
					// The damage reduction logic is in the Enemy Attack action.

					redoChoice = false;
				}

	
				else if (choice == 3) // Action 3, Heal
				{
					if (mana < 20)
					{
						std::cout << "\nYou do not have the Mana to cast this Spell.\n";

						redoChoice = true;
					}
					else
					{
						health += 30;
						if (health > maxHealth) { health = maxHealth; } // ensures player health does not surpass it's limit
						mana -= 20;

						std::cout << "You healed 30 health, your total health is now " << eHealth << "!\n";
						redoChoice = false;
					}
				}


				else if (choice == 4) // Action 4, Mana Blast
				{
					std::cout << "\nYou chose to cast Mana Blast!\n";

					if (mana < 30)
					{
						std::cout << "You do not have the Mana to cast this Spell.\n";

						redoChoice = true;
					}

					else
					{
						mana -= 30;

						int bonus = playerDamDist(generator);
						int dam = (bd + bonus) * 2; // this spell has a 2x damage multiplier

						eHealth -= dam;

						std::cout << "You cast Mana Blast, dealing " << dam << " damage to the enemy!\n";

						redoChoice = false;
					}
				}


				else if (choice == 5) // Action 5, Recover Mana
				{
					std::cout << "\nYou chose to recover your Mana!\n";

					mana += (manaRegen * 2); // restores double the natural mana regen of the enemy

					std::cout << "You restored " << (manaRegen * 2) << " Mana!\n";
					redoChoice = false;
				}
			}
		}
		

		// --- ENEMY CHOICE LOOP ---
		if (eHealth > 0)
		{
			bool redoChoice = true;

			std::cout << "The enemy is choosing their action...\n";

			while (redoChoice)
			{
				// Enemy Choice Logic	
				ec = enemyChoiceRoll(generator); // chooses an action randomly for the enemy


				if (ec == 1) // Action 1, Defend
				{
					if (ppc == 4 || ppc == 1)
					{
						std::cout << "The enemy has chosen to Defend!\n";

						std::cout << "Attacking them will reduce your damage by 4.\n";

						canBlock = true;
						redoChoice = false;
					}

					else
					{
						canBlock = false;
						redoChoice = true;
					}
				}


				else if (ec == 2) // Action 2, Heal
				{
					std::cout << "The enemy has chosen to cast a Healing Spell!\n";

					if (eMana < 20)
					{
						std::cout << "...but it didn't have enough Mana.\n";

						redoChoice = true;
					}
					else
					{
						eHealth += 25;
						if (eHealth > enemyMaxHealth) { eHealth = enemyMaxHealth; } // ensures enemy health does not surpass it's limit
						eMana -= 20;

						std::cout << "The enemy healed 25 health, it's total health is now " << eHealth << "!\n";

						redoChoice = false;
					}
				}


				else if (ec == 3) // Action 3, Mana Blast
				{
					std::cout << "The enemy has chosen to cast Mana Blast!\n";

					if (eMana < 30)
					{
						std::cout << "...but it didn't have enough Mana.\n";

						redoChoice = true;
					}

					int bonus = enemyDamDist(generator);
					int dam = (ebd + bonus) * 2; // this spell has a 2x damage multiplier

					health -= dam;

					redoChoice = false;
				}


				else if (ec == 4) // Action 4, Recover Mana
				{
					if (eMana < 100)
					{
						std::cout << "The enemy has chosen to recover it's Mana!\n";

						eMana += (eMReg * 2); // restores double the natural mana regen of the enemy

						std::cout << "The enemy rested and recovered " << (eMReg * 2) << " Mana!\n";

						redoChoice = false;
					}
					else
					{
						redoChoice = true;
					}
				}

				else // Action 5, Attack
				{
					std::cout << "The enemy has chosen to Attack!\n";

					int bonus = enemyDamDist(generator); // generates a damage multiplier for the attack's damage
					int dam = ebd + bonus; // uses the rolled damage multiplier to increase damage (and make it random)

					if (choice == 2)
					{
						dam -= 15; // reduces enemy damage by 15 if the player Defended
					}

					health -= dam; // hit damage
					std::cout << "The enemy Attacks you for " << dam << " damage!\n";

					redoChoice = false;
				}
			}
		}

		// --- RESTART LOGIC ---
		while (health <= 0 or eHealth <= 0)
		{
			if (health <= 0)
			{
				std::cout << "You were defeated!\n";
				std::cout << "The battle lasted for " << roundNum << " rounds!\n";
			}

			else if (eHealth <= 0)
			{
				std::cout << "The enemy is defeated!\n";
				std::cout << "The battle lasted for " << roundNum << " rounds!\n";
			}

			std::cout << "\n\n--------------------\n";
			std::cout << "Would you like to play again?\nY/N?\nChoose: ";
			std::cin >> restartChoice;

			if (restartChoice == "y" || restartChoice == "Y") { main(); }
			else if (restartChoice == "n" || restartChoice == "N") { gameRunning = false; }

			while (restartChoice != "y" && restartChoice != "Y" && restartChoice != "n" && restartChoice != "N") 
			{
				std::cout << "That answer is invalid.";
				std::cout << "Would you like to play again?\nY/N?\nChoose: ";
				std::cin >> restartChoice;

				if (restartChoice == "y" || restartChoice == "Y") { main(); }
				else if (restartChoice == "n" || restartChoice == "N") { gameRunning = false; }
			}
			return 0;
		}
	}
}