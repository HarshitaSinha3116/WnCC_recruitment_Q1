The attached code read four .csv files such as mentors.csv, students.csv, interactions.csv and feedbacks.csv and based on these files outputs a file called mentor_scores.csv containing quantities such as MentorID, name , final score and thus the rank of all the enlisted mentors keeping in mind that mentors having same score are assigned the same rank.


The given csv files contains the following:-
1) mentor.csv:-


   a)MentorID


   b)Name


   c)Domain


   d)projects

   
2)students.csv:-


  a)studentID

  
  b)name

  
  c)ProjectID

  
  d)NilestonesCompleted

  
  e)TotalMilestones

  
3)interaction.csv:-


  a)MentorID

  
  b)StudentID

  
  c)Meetings

  
  d)CodeReviews

  
  e)Messages

  
  f)AvgResponseTime

  
4)feedbacks.csv:-


  a)MentorID

  
  b)StudentID

  
  c)Rating
  

What my code does is that it calculates four parameters which are student procress score, Responsiveness Score, Engagement Score and Mentee Feedback Score and takes there weighted average as the final score which is normalised among mentors, takes into account time decay, consistancy of mentors and honesty of students feedbacks.


I have written my code in VScode so in order to run my code please keeo all the 4 csv file along with the code.cpp in the same folder, do open with code and run the code.
