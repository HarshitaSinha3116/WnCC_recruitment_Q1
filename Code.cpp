#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

struct Mentor{
    string MentorID, Name, Domain, Projects;
};

struct Student{
    string StudentID, Name, ProjectID;
    int MilestonesCompleted, TotalMilestones;
};

struct Interaction{
    string MentorID, StudentID;
    int Meetings, CodeReviews, Messages;
    double AvgResponseTime;
};

struct Feedback{
    string MentorID, StudentID;
    int Rating;
};

struct MentorScore{
    string MentorID, Name;
    double P, R, E, F, FinalScore;
    double PreviousScore;
    int Rank;
};

vector<Mentor> readMentors(const string &filename){
    vector<Mentor> mentors;
    ifstream file(filename);
    string line;
    getline(file, line);

    while (getline(file, line)){
        stringstream ss(line);
        Mentor m;
        getline(ss, m.MentorID, ',');
        getline(ss, m.Name, ',');
        getline(ss, m.Domain, ',');
        getline(ss, m.Projects, ',');
        mentors.push_back(m);
    }
    return mentors;
}

vector<Student> readStudents(const string &filename){
    vector<Student> students;
    ifstream file(filename);
    string line;
    getline(file, line);

    while (getline(file, line)){
        stringstream ss(line);

        Student St;
        getline(ss, St.StudentID, ',');
        getline(ss, St.Name, ',');
        getline(ss, St.ProjectID, ',');

        string temp1, temp2;
        getline(ss, temp1, ',');
        getline(ss, temp2, ',');
        St.MilestonesCompleted = stoi(temp1);
        St.TotalMilestones = stoi(temp2);

        students.push_back(St);
    }
    return students;
}

vector<Interaction> readInteractions(const string &filename){
    vector<Interaction> interactions;
    ifstream file(filename);
    string line;
    getline(file, line);

    while (getline(file, line)){
        stringstream ss(line);

        Interaction inter;
        getline(ss, inter.MentorID, ',');
        getline(ss, inter.StudentID, ',');

        string t1, t2, t3, t4;
        getline(ss, t1, ',');
        getline(ss, t2, ',');
        getline(ss, t3, ',');
        getline(ss, t4, ',');
        inter.Meetings = stoi(t1);
        inter.CodeReviews = stoi(t2);
        inter.Messages = stoi(t3);
        inter.AvgResponseTime = stod(t4);

        interactions.push_back(inter);
    }
    return interactions;
}

vector<Feedback> readFeedbacks(const string &filename){
    vector<Feedback> feedbacks;
    ifstream file(filename);
    string line;
    getline(file, line);

    while (getline(file, line)){
        stringstream ss(line);

        Feedback f;
        getline(ss, f.MentorID, ',');
        getline(ss, f.StudentID, ',');

        string f1;
        getline(ss, f1, ',');
        f.Rating = stoi(f1);

        feedbacks.push_back(f);
    }
    return feedbacks;
}

double computeP(const vector<Student> &mentorStudents){
    int totalCompleted = 0;
    int totalAssigned = 0;

    for (auto &student : mentorStudents){
        totalCompleted += student.MilestonesCompleted;
        totalAssigned += student.TotalMilestones;
    }
    if (totalAssigned == 0)
        return 0;

    else
        return (double)totalCompleted / totalAssigned;
}

double computeR(const vector<Interaction> &mentorStudentInteractions){
    if (mentorStudentInteractions.empty())
        return 0;

    double totalResponseTime = 0;
    for (auto &inter : mentorStudentInteractions){
        totalResponseTime += inter.AvgResponseTime;
    }

    double avgTime = totalResponseTime / mentorStudentInteractions.size();

    return 1.0 / (1.0 + avgTime);
}

double computeE(const vector<Interaction> &mentorStudentInteractions){
    if (mentorStudentInteractions.empty())
        return 0;

    int menteeCount = mentorStudentInteractions.size();

    double totalMeetings = 0;
    double totalCodeReviews = 0;
    double totalMessages = 0;

    for (auto &inter : mentorStudentInteractions){

        totalMeetings += inter.Meetings;
        totalCodeReviews += inter.CodeReviews;
        totalMessages += inter.Messages;
    }

    double avgMeetings = totalMeetings / menteeCount;
    double avgCodeReviews = totalCodeReviews / menteeCount;
    double avgMessages = totalMessages / menteeCount;

    double p1 = 0.25, p2 = 0.4, p3 = 0.35;
    double E = (p1 * avgMeetings) + (p2 * avgCodeReviews) + (p3 * avgMessages);

    return 1.0 - exp(-E);
}

double computeF(const vector<Feedback> &mentorFeedback){
    if (mentorFeedback.empty())
        return 0;

    vector<int> ratings;
    for (auto &fb : mentorFeedback){
        ratings.push_back(fb.Rating);
    }

    double sum = 0;
    for (int r : ratings)
        sum += r;
    double mean = sum / ratings.size();

    double variance = 0;
    for (int r : ratings){
        variance += (r - mean) * (r - mean);
    }
    double stddev = sqrt(variance / ratings.size());

    vector<int> cleanRatings;
    for (int r : ratings){
        if (abs(r - mean) <= 2 * stddev){
            cleanRatings.push_back(r);
        }
    }

    if (cleanRatings.empty())
        cleanRatings = ratings;

    double cleanSum = 0;
    for (int r : cleanRatings)
        cleanSum += r;
    double cleanAvg = cleanSum / cleanRatings.size();

    return (cleanAvg - 1) / 4;
}

double ScoreOverTime(double previousScore, double currentScore){
    double alpha = 0.6;
    return (1 - alpha) * previousScore + alpha * currentScore;
}

double applyDecay(double currentScore, bool isInactive){
    double d = 0.3;

    if (isInactive){
        return currentScore * (1 - d);
    }
    else{
        return currentScore;
    }
}

double computeFinalScore(double P, double R, double E, double F){
    double w1 = 0.35, w2 = 0.20, w3 = 0.30, w4 = 0.15;
    return w1 * P + w2 * R + w3 * E + w4 * F;
}

void writeOutput(vector<MentorScore> &scores, const string &filename){
    sort(scores.begin(), scores.end(), [](const MentorScore &a, const MentorScore &b)
         { return a.FinalScore > b.FinalScore; });

    int rank = 1;
    scores[0].Rank = rank;

    for (int i = 0; i < scores.size(); i++){
        if (abs(scores[i].FinalScore - scores[i - 1].FinalScore) < 1e-6){

            scores[i].Rank = scores[i - 1].Rank;
        }
        else{
            scores[i].Rank = i + 1;
        }
    }

    ofstream outFile(filename);

    outFile << "MentorID,Name,FinalScore,Rank" << "\n";

    for (auto &score : scores){
        outFile << score.MentorID << ","
                << score.Name << ","
                << score.FinalScore << ","
                << score.Rank << "\n";
    }

    outFile.close();
}

int main(){
    auto mentors = readMentors("mentors.csv");
    auto students = readStudents("students.csv");
    auto interactions = readInteractions("interactions.csv");
    auto feedbacks = readFeedbacks("feedbacks.csv");

    vector<MentorScore> results;
    map<string, double> previousScores;

    for (auto &mentor : mentors){
        bool isInactive = true;
        for (auto &inter : interactions){
            if (inter.MentorID == mentor.MentorID){
                isInactive = false;
                break;
            }
        }

        vector<Student> mentorStudents;
        for (auto &student : students){
            for (auto &inter : interactions){
                if (inter.MentorID == mentor.MentorID && inter.StudentID == student.StudentID){
                    mentorStudents.push_back(student);
                    break;
                }
            }
        }

        vector<Interaction> mentorStudentInteractions;
        for (auto &inter : interactions){
            if (inter.MentorID == mentor.MentorID){
                mentorStudentInteractions.push_back(inter);
            }
        }

        vector<Feedback> mentorFeedback;
        for (auto &fb : feedbacks){
            if (fb.MentorID == mentor.MentorID){
                mentorFeedback.push_back(fb);
            }
        }

        double P = computeP(mentorStudents);
        double R = computeR(mentorStudentInteractions);
        double E = computeE(mentorStudentInteractions);
        double F = computeF(mentorFeedback);

        double currentScore = computeFinalScore(P, R, E, F);
        double prevScore = previousScores[mentor.MentorID];
        double smoothedScore = ScoreOverTime(prevScore, currentScore);
        previousScores[mentor.MentorID] = smoothedScore;

        double finalScore = applyDecay(smoothedScore, isInactive);

        MentorScore ms;
        ms.MentorID = mentor.MentorID;
        ms.Name = mentor.Name;
        ms.P = P;
        ms.R = R;
        ms.E = E;
        ms.F = F;
        ms.FinalScore = finalScore;
        results.push_back(ms);
    }

    writeOutput(results, "mentor_scores.csv");
    return 0;
}
