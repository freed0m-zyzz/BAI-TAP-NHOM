#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//Khai Bao

enum class WorkoutMode {
    Gym,
    Home
};

enum class Mechanic {
    Compound,
    Isolation
};

enum class Difficulty {
    Beginner,
    Intermediate,
    Advanced
};

class Exercise {
private:
    string name;
    Mechanic mechanic;
    Difficulty difficulty;
    int sets;
    int reps;
    double weightKg;

public:
    Exercise();
    Exercise(const string& name,
             Mechanic mechanic,
             Difficulty difficulty,
             int sets,
             int reps,
             double weightKg);

    const string& getName() const;
    Mechanic getMechanic() const;
    Difficulty getDifficulty() const;
    int getSets() const;
    int getReps() const;
    double getWeightKg() const;

    void setName(const string& value);
    void setMechanic(Mechanic value);
    void setDifficulty(Difficulty value);
    void setSets(int value);
    void setReps(int value);
    void setWeightKg(double value);

    double calculateVolume(WorkoutMode mode) const;
};

class WorkoutSession {
private:
    string dateTime;
    WorkoutMode mode;
    vector<Exercise> exercises;

public:
    WorkoutSession();
    WorkoutSession(const string& dateTime, WorkoutMode mode);

    const string& getDateTime() const;
    WorkoutMode getMode() const;
    const vector<Exercise>& getExercises() const;

    void setDateTime(const string& value);
    void setMode(WorkoutMode value);
    void addExercise(const Exercise& exercise);

    double getTotalSessionVolume() const;
};

class WorkoutManager {
private:
    vector<WorkoutSession> sessions;

public:
    void addSession(const WorkoutSession& session);
    vector<WorkoutSession> getAllSessions() const;
    bool saveToFile(const string& filename) const;
    bool loadFromFile(const string& filename);
};

// ========================= IMPLEMENTATION (Definitions) =========================

namespace {
string trim(const string& s) {
    const string ws = " \t\n\r";
    size_t start = s.find_first_not_of(ws);
    if (start == string::npos) {
        return "";
    }
    size_t end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

vector<string> split(const string& line, char delimiter) {
    vector<string> parts;
    string token;
    stringstream ss(line);
    while (getline(ss, token, delimiter)) {
        parts.push_back(token);
    }
    return parts;
}

string modeToString(WorkoutMode mode) {
    return mode == WorkoutMode::Gym ? "Gym" : "Home";
}

string mechanicToString(Mechanic mechanic) {
    return mechanic == Mechanic::Compound ? "Compound" : "Isolation";
}

string difficultyToString(Difficulty difficulty) {
    if (difficulty == Difficulty::Beginner) {
        return "Beginner";
    }
    if (difficulty == Difficulty::Intermediate) {
        return "Intermediate";
    }
    return "Advanced";
}

bool stringToMode(const string& text, WorkoutMode& out) {
    if (text == "Gym") {
        out = WorkoutMode::Gym;
        return true;
    }
    if (text == "Home") {
        out = WorkoutMode::Home;
        return true;
    }
    return false;
}

bool stringToMechanic(const string& text, Mechanic& out) {
    if (text == "Compound") {
        out = Mechanic::Compound;
        return true;
    }
    if (text == "Isolation") {
        out = Mechanic::Isolation;
        return true;
    }
    return false;
}

bool stringToDifficulty(const string& text, Difficulty& out) {
    if (text == "Beginner") {
        out = Difficulty::Beginner;
        return true;
    }
    if (text == "Intermediate") {
        out = Difficulty::Intermediate;
        return true;
    }
    if (text == "Advanced") {
        out = Difficulty::Advanced;
        return true;
    }
    return false;
}
} // namespace

Exercise::Exercise()
    : mechanic(Mechanic::Compound), difficulty(Difficulty::Beginner), sets(0), reps(0), weightKg(0.0) {}

Exercise::Exercise(const string& name,
                   Mechanic mechanic,
                   Difficulty difficulty,
                   int sets,
                   int reps,
                   double weightKg)
    : name(name), mechanic(mechanic), difficulty(difficulty), sets(sets), reps(reps), weightKg(weightKg) {}

const string& Exercise::getName() const {
    return name;
}

Mechanic Exercise::getMechanic() const {
    return mechanic;
}

Difficulty Exercise::getDifficulty() const {
    return difficulty;
}

int Exercise::getSets() const {
    return sets;
}

int Exercise::getReps() const {
    return reps;
}

double Exercise::getWeightKg() const {
    return weightKg;
}

void Exercise::setName(const string& value) {
    name = value;
}

void Exercise::setMechanic(Mechanic value) {
    mechanic = value;
}

void Exercise::setDifficulty(Difficulty value) {
    difficulty = value;
}

void Exercise::setSets(int value) {
    sets = value;
}

void Exercise::setReps(int value) {
    reps = value;
}

void Exercise::setWeightKg(double value) {
    weightKg = value;
}

double Exercise::calculateVolume(WorkoutMode mode) const {
    if (mode == WorkoutMode::Home || weightKg == 0.0) {
        return static_cast<double>(sets) * reps;
    }
    return static_cast<double>(sets) * reps * weightKg;
}

WorkoutSession::WorkoutSession() : mode(WorkoutMode::Gym) {}

WorkoutSession::WorkoutSession(const string& dateTime, WorkoutMode mode)
    : dateTime(dateTime), mode(mode) {}

const string& WorkoutSession::getDateTime() const {
    return dateTime;
}

WorkoutMode WorkoutSession::getMode() const {
    return mode;
}

const vector<Exercise>& WorkoutSession::getExercises() const {
    return exercises;
}

void WorkoutSession::setDateTime(const string& value) {
    dateTime = value;
}

void WorkoutSession::setMode(WorkoutMode value) {
    mode = value;
}

void WorkoutSession::addExercise(const Exercise& exercise) {
    exercises.push_back(exercise);
}

double WorkoutSession::getTotalSessionVolume() const {
    double total = 0.0;
    for (size_t i = 0; i < exercises.size(); ++i) {
        total += exercises[i].calculateVolume(mode);
    }
    return total;
}

void WorkoutManager::addSession(const WorkoutSession& session) {
    sessions.push_back(session);
}

vector<WorkoutSession> WorkoutManager::getAllSessions() const {
    return sessions;
}

bool WorkoutManager::saveToFile(const string& filename) const {
    ofstream fout(filename.c_str(), ios::trunc);
    if (!fout.is_open()) {
        return false;
    }

    for (size_t i = 0; i < sessions.size(); ++i) {
        const WorkoutSession& session = sessions[i];
        fout << "SESSION;" << session.getDateTime() << ";" << modeToString(session.getMode()) << "\n";

        const vector<Exercise>& list = session.getExercises();
        for (size_t j = 0; j < list.size(); ++j) {
            const Exercise& ex = list[j];
            fout << "EX;" << ex.getName() << ";" << mechanicToString(ex.getMechanic()) << ";"
                 << difficultyToString(ex.getDifficulty()) << ";" << ex.getSets() << ";" << ex.getReps() << ";"
                 << fixed << setprecision(2) << ex.getWeightKg() << "\n";
        }

        fout << "END\n";
    }

    return true;
}

bool WorkoutManager::loadFromFile(const string& filename) {
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        sessions.clear();
        return false;
    }

    vector<WorkoutSession> loaded;
    WorkoutSession current;
    bool inSession = false;
    string line;

    while (getline(fin, line)) {
        line = trim(line);
        if (line.empty()) {
            continue;
        }

        vector<string> parts = split(line, ';');
        if (parts.empty()) {
            continue;
        }

        if (parts[0] == "SESSION" && parts.size() >= 3) {
            if (inSession) {
                loaded.push_back(current);
            }
            WorkoutMode mode;
            if (!stringToMode(parts[2], mode)) {
                continue;
            }
            current = WorkoutSession(parts[1], mode);
            inSession = true;
        } else if (parts[0] == "EX" && parts.size() >= 7 && inSession) {
            Mechanic mechanic;
            Difficulty difficulty;
            if (!stringToMechanic(parts[2], mechanic) || !stringToDifficulty(parts[3], difficulty)) {
                continue;
            }

            try {
                int sets = stoi(parts[4]);
                int reps = stoi(parts[5]);
                double weightKg = stod(parts[6]);
                current.addExercise(Exercise(parts[1], mechanic, difficulty, sets, reps, weightKg));
            } catch (...) {
                continue;
            }
        } else if (parts[0] == "END" && inSession) {
            loaded.push_back(current);
            inSession = false;
        }
    }

    if (inSession) {
        loaded.push_back(current);
    }

    sessions = loaded;
    return true;
}

// ========================= Demo Frontend Gia Lap =========================

int main() {
    WorkoutManager manager;

    WorkoutSession gymSession("2026-04-08 18:30:00", WorkoutMode::Gym);
    gymSession.addExercise(Exercise("Bench Press", Mechanic::Compound, Difficulty::Intermediate, 4, 8, 60.0));
    gymSession.addExercise(Exercise("Tricep Pushdown", Mechanic::Isolation, Difficulty::Beginner, 3, 12, 20.0));

    WorkoutSession homeSession("2026-04-09 07:00:00", WorkoutMode::Home);
    homeSession.addExercise(Exercise("Push Up", Mechanic::Compound, Difficulty::Beginner, 4, 20, 0.0));
    homeSession.addExercise(Exercise("Bodyweight Squat", Mechanic::Compound, Difficulty::Beginner, 3, 25, 0.0));

    manager.addSession(gymSession);
    manager.addSession(homeSession);

    const string fileName = "fitness_log.txt";
    bool saved = manager.saveToFile(fileName);

    WorkoutManager loadedManager;
    bool loaded = loadedManager.loadFromFile(fileName);
    vector<WorkoutSession> sessions = loadedManager.getAllSessions();

    cout << "Saved: " << (saved ? "true" : "false") << "\n";
    cout << "Loaded: " << (loaded ? "true" : "false") << "\n";
    cout << "Session count: " << sessions.size() << "\n";

    for (size_t i = 0; i < sessions.size(); ++i) {
        cout << "- " << sessions[i].getDateTime() << " | " << modeToString(sessions[i].getMode())
             << " | Total Volume=" << fixed << setprecision(2) << sessions[i].getTotalSessionVolume() << "\n";
    }

    return 0;
}
