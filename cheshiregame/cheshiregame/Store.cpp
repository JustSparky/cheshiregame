#include "stdafx.h"
#include "Player.h"
#include "Store.h"
#include "Item.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>

Store::Store()
	: numHealthPots(0), numManaPots(0)
{}
Store::~Store()
{}
void Store::fillStore(const string &input_file)
{
	ifstream fin;
	string store_file = "Assets/Items/" + input_file;

	string itemName;
	int val;
	string type;
	int boost;
	int num_potions = 0;

	fin.open(store_file.c_str());
	if (!fin)
	{
		std::cout << "Error opening " << store_file << std::endl;
		return;
	}

	while (fin >> type)
	{
		if (type == "Weapon")
		{
			fin.ignore(); getline(fin, itemName); fin >> boost; fin >> val;
			Weapon* temp = new Weapon(itemName, type, boost, val);
			storeInventory.push_back(temp);
			weapInv.push_back(temp);
		}
		else if (type == "Gear")
		{
			fin >> type; fin.ignore(); getline(fin, itemName); fin >> val; fin >> boost;
			Gear* temp = new Gear(itemName, val, type, boost);
			storeInventory.push_back(temp);
			gearInv.push_back(temp);
		}
		else if (type == "Consumable")
		{
			fin.ignore(); getline(fin, itemName); fin >> val; fin >> boost; fin >> num_potions;
			for (int i = 0; i < num_potions; ++i)
			{
				Consumable* temp = new Consumable(itemName, type, boost, val);
				if (!healthPotExists(storeInventory) && itemName == "Health Potion")
				{
					storeInventory.push_back(temp);
				}
				else if (!manaPotExists(storeInventory) && itemName == "Mana Potion")
				{
					storeInventory.push_back(temp);
				}
				if (itemName == "Health Potion")
				{
					++numHealthPots;
				}
				else if (itemName == "Mana Potion")
				{
					++numManaPots;
				}
				potInv.push_back(temp);
			}
		}
	}
	fin.close();
}
void Store::menu() const
{
	std::cout << "1. View store inventory." << std::endl;
	std::cout << "2. Purchase item." << std::endl;
	std::cout << "3. Sell item." << std::endl;
	std::cout << "4. Exit." << std::endl;
}

void Store::run(player* p)
{
	string choice = "0";
	int usernum;

	std::cout << "Gold remaining: " << p->getmoney() << std::endl;
	menu();
	cin >> choice;

	while (choice != "4")
	{
		while (cin.fail())
		{
			std::cout << "Invalid input.\n";
			cin >> choice;
		}
		if (choice == "1")
		{
			system("cls");
			for (int i = 0; i < storeInventory.size(); ++i)
			{
				std::cout << i + 1 << ": " << storeInventory.at(i)->getName() << std::endl;
				std::cout << storeInventory.at(i)->getValue() << " gold" << std::endl;

				if (storeInventory.at(i)->getName() == "Health Potion")
				{
					std::cout << "Amount: " << numHealthPots << std::endl;
				}
				else if (storeInventory.at(i)->getName() == "Mana Potion")
				{
					std::cout << "Amount: " << numManaPots << std::endl;
				}
			}
			menu();
			cin >> choice;
		}
		else if (choice == "2")
		{
			system("cls");
			std::cout << "Enter the number of the item to purchase." << std::endl;
			std::cin >> usernum;
			int goldSpent = 0;
			if (usernum > storeInventory.size() || usernum < 1)
			{
				std::cout << "Item cannot be accessed." << std::endl;
				menu();
				cin >> choice;
			}
			else if (p->getmoney() < storeInventory.at(usernum - 1)->getValue())
			{
				std::cout << "You are too poor." << std::endl;
				menu();
				cin >> choice;
			}
			else
			{
				if (storeInventory.at(usernum - 1)->getType() == "Weapon")
				{
					for (int i = 0; i < weapInv.size(); ++i)
					{
						if (storeInventory.at(usernum - 1)->getName() == weapInv.at(i)->getName())
						{
							p->add_inventory(weapInv.at(i));
						}
					}
					std::cout << "You bought one " << storeInventory.at(usernum - 1)->getName() << "." << std::endl;

					goldSpent += storeInventory.at(usernum - 1)->getValue();
					int playerMoney = p->getmoney() - storeInventory.at(usernum - 1)->getValue();
					p->setmoney(playerMoney);

					storeInventory.erase(storeInventory.begin() + usernum - 1);

					std::cout << "You have " << p->getmoney() << " gold left." << std::endl;
					menu();
					cin >> choice;
				}
				else if (storeInventory.at(usernum - 1)->getType() == "Gear")
				{
					for (int i = 0; i < gearInv.size(); ++i)
					{
						if (storeInventory.at(usernum - 1)->getName() == gearInv.at(i)->getName())
						{
							p->add_inventory(gearInv.at(i));
						}
					}
					std::cout << "You bought one " << storeInventory.at(usernum - 1)->getName() << "." << std::endl;

					goldSpent += storeInventory.at(usernum - 1)->getValue();
					int playerMoney = p->getmoney() - storeInventory.at(usernum - 1)->getValue();
					p->setmoney(playerMoney);

					storeInventory.erase(storeInventory.begin() + usernum - 1);

					std::cout << "You have " << p->getmoney() << " gold left." << std::endl;
					menu();
					cin >> choice;
				}
				else if (storeInventory.at(usernum - 1)->getType() == "Consumable")
				{
					if (storeInventory.at(usernum - 1)->getName() == "Health Potion")
					{
						for (int i = 0; i < potInv.size(); ++i)
						{
							if (storeInventory.at(usernum - 1)->getName() == potInv.at(i)->getName())
							{
								p->add_inventory(potInv.at(i));
								potInv.erase(potInv.begin() + i);
							}
						}
						--numHealthPots;
						std::cout << "You bought one " << storeInventory.at(usernum - 1)->getName() << "." << std::endl;

						goldSpent += storeInventory.at(usernum - 1)->getValue();
						int playerMoney = p->getmoney() - storeInventory.at(usernum - 1)->getValue();
						p->setmoney(playerMoney);

						std::cout << "You have " << p->getmoney() << " gold left." << std::endl;

						if (numHealthPots == 0)
						{
							storeInventory.erase(storeInventory.begin() + usernum - 1);
						}
						menu();
						cin >> choice;
					}
					else //Mana pot
					{
						for (int i = 0; i < potInv.size(); ++i)
						{
							if (storeInventory.at(usernum - 1)->getName() == potInv.at(i)->getName())
							{
								p->add_inventory(potInv.at(i));
								potInv.erase(potInv.begin() + i);
							}
						}
						--numManaPots;
						std::cout << "You bought one " << storeInventory.at(usernum - 1)->getName() << "." << std::endl;

						goldSpent += storeInventory.at(usernum - 1)->getValue();
						int playerMoney = p->getmoney() - storeInventory.at(usernum - 1)->getValue();
						p->setmoney(playerMoney);

						std::cout << "You have " << p->getmoney() << " gold left." << std::endl;

						if (numManaPots == 0)
						{
							storeInventory.erase(storeInventory.begin() + usernum - 1);
						}
						menu();
						cin >> choice;
					}
				}
			}
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
		else if (choice == "3")
		{
			system("cls");
			p->display_inventory();
			std::cout << "Enter the number of the item you would like to sell.\n";
			std::cin >> usernum;
			if (p->getInvSize() == 0)
			{
				std::cout << "Inventory empty.\n";
				menu();
				cin >> choice;
			}
			if (usernum >= p->getInvSize()) {
				std::cout << "Error: Item does not exist.\n";
				menu();
				cin >> choice;
			}
			if (usernum < 0) {
				std::cout << "Error: Invalid inventory position.\n";
				menu();
				cin >> choice;
			}

			storeInventory.push_back(p->inventory.at(usernum));

			int totalGold = p->getmoney() + p->inventory.at(usernum)->getValue();
			p->setmoney(totalGold);

			p->inventory.erase(p->inventory.begin() + usernum);
			std::cout << "Gold remaining: " << p->getmoney() << std::endl;
			menu();
			cin >> choice;
		}
    }
    std::cout << "Gold remaining: " << p->getmoney() << std::endl;
    return;
}
void Store::storeUpdate()
{
	std::cout << "Our shipments are still on their way!" << std::endl;
	return;
}
bool Store::healthPotExists(const vector<Item*>& v)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (v.at(i)->getName() == "Health Potion")
		{
			return true;
		}
	}
	return false;
}
bool Store::manaPotExists(const vector<Item*>& v)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (v.at(i)->getName() == "Mana Potion")
		{
			return true;
		}
	}
	return false;
}