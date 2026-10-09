#include <iostream>;
using namespace std;

#include <random>;
std::default_random_engine generator;

std::uniform_int_distribution<int> dFour(1, 4); // roll 1d4
std::uniform_int_distribution<int> dSix(1, 6); // roll 1d6
std::uniform_int_distribution<int> dEight(1, 8); // roll 1d8
std::uniform_int_distribution<int> dTen(1, 10); // roll 1d10
std::uniform_int_distribution<int> dTwelve(1, 12); // roll 1d12
std::uniform_int_distribution<int> dTwenty(1, 20); // roll 1d20

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
string restartChoice = "nil";
int roundNum = 0;

int playerConsumables[3]; // player item inventory

// signals the game that it is running
bool gameRunning = true;

// CLASSES
class Character
{
public:
	// advanced stats
	int maxHealth; // (5 + endurance) * proficiencyBonus
	int health;
	int healthRegen; // (1 + (charisma / 2)) * (proficiencyBonus / 2)
	int maxMana; // 100 + (2 * intelligence)
	int mana;
	int manaRegen; // (intelligence / 2) + proficiencyBonus + ((maxMana * intelligence) / 100)
	int armourClass; // armourBonus
	int meleeDamage; // weaponBonus + strength
	int meleeRollBonus; // proficiencyBonus
	int spellDamage; // weaponBonus + (intelligence * 2)
	int spellRollBonus; // proficiencyBonus
	int critChance; // 1 (5%)
	int bonusCritDamage; // 0 (Crits always roll dice damage a second time)
	int actions; // 1 + (proficiencyBonus / 2)

	// base stats
	int strength; // gain +1 Melee Damage for every other point
	int perception; // increase damage on Crit by +1 per point
	int endurance; // gain +1 Armour Class for every other point
	int charisma; // gain +1 Spell Attack Rolls for every other point
	int intelligence; // increase Spell Damage by +1 per point
	int agility; // gain +1 Melee Attack rolls for every other point
	int luck; // increase Crit Chance by +1 for every other point

	int proficiencyBonus; // increases a number of things, +1 proficiency bonus for every 5 levels starting with +2

	// levelling
	int statPoints; // start with 10 + 1 per level (11 at level 1)
	int spentPoints; // record of points spent on character
	int characterLevel;
	int experience; // current experience gained this level
	int expToLvlUp; // experience required to level up

	// inventory
	string SlotHead[1]; // ...armour slots
	string SlotTorso[1]; // ...
	string SlotLegs[1]; // ...armour slots

	string SlotWeapons[1]; // weapon slots
	string SlotConsumables[3]; // consumable gear slots

	string Inventory[20]; // 20 inventory slots as storage

};

class Item
{
	public:
		string name;
		std::vector<string> itemType;
		std::vector<string> itemAttributes;
		std::vector<int> GearBonuses;
};

// ITEM LIST

// Weapons
int nextID = 0;
std::vector<Item> weapons;


// FUNCTIONS

void DisplayStats(int pH, int pM, int eH, int eM)
{
	// player stats
	std::cout << "\nPlayer Health: " << pH << endl;
	std::cout << "\nPlayer Mana: " << pM << endl;
	// enemy stats
	std::cout << "\nEnemy Health: " << eH << endl;
	std::cout << "\nEnemy Mana: " << eM << endl;
}

int ApplyDamage(int h, int d)
{
	h -= d;

	if (h < 0) { h = 0; } // assures health does not drop into the negatives

	return h;
}

int RollDamage(int rollNum, std::uniform_int_distribution<int> dice, int bd)
{
	int d = 0;

	for (int i = 0; i < rollNum; i++)
	{

		int d = d + dice(generator);
	}

	return d;
}

bool IsAlive(int h)
{
	return h > 0;
}

// GAME LOOP

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
				DisplayStats(health, mana, eHealth, eMana);

				std::cout << "\n1. Attack\n2. Defend\n3. Heal (Costs 20 Mana)\n4. Mana Blast (Costs 30 Mana)\n5. Recover Mana\n6. View Inventory\nChoose: "; // asks for the player's choice
				std::cin >> choice; // collects the player's choice



				if (choice < 1 || choice > 6)
				{
					std::cout << "\nThis choice is invalid, please choose another.\n";

					redoChoice = true;
				}


				else if (choice == 1) // Action 1, Attack
				{
					std::cout << "\nYou Attacked the enemy with your weapon!\n";

					int dam = RollDamage(playerDamDist, bd, 1); // rolls the player's attack damage

					if (ec == 2 && canBlock == true)
					{
						dam -= 8; // reduces attack damage by 8 if enemy Defended
					}

					eHealth = ApplyDamage(eHealth, dam);

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

						// this spell has a 2x damage multiplier
						int dam = RollDamage(playerDamDist, bd, 2);

						eHealth = ApplyDamage(eHealth, dam);

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

				else if (choice == 6) // Action 6, View Inventory
				{
					void;
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

					eMana -= 30;
					
					// this spell has a 2x damage multiplier
					int dam = RollDamage(enemyDamDist, ebd, 2);

					health = ApplyDamage(health, dam);

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

					int dam = RollDamage(enemyDamDist, ebd, 1); // rolls the enemy's attack damage

					if (choice == 2)
					{
						dam -= 15; // reduces enemy damage by 15 if the player Defended
					}

					health = ApplyDamage(health, dam);

					std::cout << "The enemy Attacks you for " << dam << " damage!\n";

					redoChoice = false;
				}
			}
		}

		bool PAlive = IsAlive(health);
		bool EAlive = IsAlive(eHealth);

		// --- RESTART LOGIC ---
		while (PAlive == false || EAlive == false && gameRunning == true)
		{
			if (PAlive == false)
			{
				std::cout << "You were defeated!\n";
				std::cout << "The battle lasted for " << roundNum << " rounds!\n";
			}

			else if (EAlive == false)
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
		}
	}
}