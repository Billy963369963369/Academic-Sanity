
#include <iostream>
#include <string>
#include <random>
using namespace std;

// =====================================
// RANDOM NUMBER GENERATOR
// =====================================

random_device rd;
mt19937 rng(rd());
uniform_int_distribution<int> dist(1, 100);

// =====================================
// BASE CLASS: CHARACTER
// =====================================

class Character {
protected:
    int health;
    int damage;

public:
    Character(int hp, int dmg)
        : health(hp), damage(dmg) {}

    virtual ~Character() = default;

    virtual int attack() {
        return damage;
    }

    bool isAlive() const {
        return health > 0;
    }

    int getHealth() const {
        return health;
    }

    void takeDamage(int amount) {
        health -= amount;

        if (health < 0) {
            health = 0;
        }
    }
};

// =====================================
// PLAYER CLASS
// =====================================

class Player : public Character {
private:
    int delusionTurns = 0;

public:
    Player(int hp, int dmg)
        : Character(hp, dmg) {}

    bool isDelusional() const {
        return delusionTurns > 0;
    }

    void heal(int amount) {
        health += amount;

        if (health > 100) {
            health = 100;
        }
    }

    void levelUp() {
        damage += 5;
        health += 20;

        if (health > 100) {
            health = 100;
        }
    }

    int attack() override {
        int finalDamage = damage;

        bool wasDelusional = isDelusional();
        int critChance = wasDelusional ? 30 : 20;

        int roll = dist(rng);

        if (roll <= critChance) {
            finalDamage *= 2;

            cout << "CRITICAL HIT!" << endl;

            if (!wasDelusional) {
                delusionTurns = 3;

                cout << "ACADEMIC DELUSION ACTIVATED!"
                     << endl;
                cout << "I definitely passed this exam..."
                     << endl;
            }
        }

        if (wasDelusional) {
            delusionTurns -= 1;

            if (delusionTurns == 0) {
                cout << "Academic Delusion expired!"
                     << endl;
            }
        }

        return finalDamage;
    }
};

// =====================================
// ENEMY CLASS
// =====================================

class Enemy : public Character {
public:
    Enemy(int hp, int dmg)
        : Character(hp, dmg) {}
};

// =====================================
// COMBAT FUNCTIONS
// =====================================

void playerAttacks(Player& player, Enemy& enemy) {
    int dealt = player.attack();

    enemy.takeDamage(dealt);

    cout << "You dealt " << dealt
         << " damage!" << endl;
}

void enemyAttacks(
    Enemy& enemy,
    Player& player,
    bool delusionActive
) {
    int dealt = enemy.attack();

    if (delusionActive) {
        dealt = static_cast<int>(dealt * 1.2);

        cout << "Academic Delusion backfires!"
             << endl;
    }

    player.takeDamage(dealt);

    cout << "The Examiner dealt " << dealt
         << " damage!" << endl;
}

// =====================================
// MAIN GAME
// =====================================

int main() {
    Player hero(100, 20);
    Enemy boss(80, 15);

    int potions = 3;
    bool running = true;

    string bossName = "The Examiner";

    cout << "===== ACADEMIC SANITY v0.2 ====="
         << endl;

    cout << "The Examiner appears!" << endl;

    while (running && hero.isAlive() && boss.isAlive()) {
        bool turnUsed = false;
        int choice;

        cout << "\n===========================" << endl;
        cout << "Your HP: " << hero.getHealth() << endl;
        cout << "Boss: " << bossName << endl;
        cout << "Boss HP: " << boss.getHealth() << endl;
        cout << "Potions: " << potions << endl;

        cout << "Academic Delusion: "
             << (hero.isDelusional() ? "ACTIVE" : "OFF")
             << endl;

        cout << "\n1. Attack" << endl;
        cout << "2. Heal" << endl;
        cout << "3. Run" << endl;
        cout << "Choose your action: ";

        if (!(cin >> choice)) {
            cout << "Invalid input!" << endl;

            if (cin.eof()) {
                break;
            }

            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        // Capture the effect state at the start
        // of this round's action.
        bool delusionActive = hero.isDelusional();

        switch (choice) {
            case 1:
                playerAttacks(hero, boss);
                turnUsed = true;
                break;

            case 2:
                if (potions > 0) {
                    hero.heal(25);
                    potions--;
                    turnUsed = true;

                    cout << "Potions remaining: "
                         << potions << endl;
                } else {
                    cout << "No potions, amigo!"
                         << endl;
                }
                break;

            case 3:
                cout << "Running away!" << endl;
                running = false;
                break;

            default:
                cout << "Invalid choice!" << endl;
                break;
        }

        // Boss counterattacks only after a
        // successfully completed player action.
        if (turnUsed && boss.isAlive() && running) {
            enemyAttacks(boss, hero, delusionActive);
        }
    }

    // =================================
    // BATTLE RESULT
    // =================================

    if (!running) {
        cout << "\nYou escaped the exam!"
             << endl;
    } else if (hero.isAlive() && !boss.isAlive()) {
        cout << "\nVICTORY! Exam passed!"
             << endl;

        hero.levelUp();
    } else if (!hero.isAlive()) {
        cout << "\nGAME OVER! See you in September."
             << endl;
    } else {
        cout << "\nBattle interrupted." << endl;
    }

    return 0;
}
