import java.io.File;
import java.io.FileNotFoundException;
import java.util.Iterator;
import java.util.PriorityQueue;
import java.util.Scanner;

/* Feifan Cao
   Dr. Steinberg
   COP3503 Spring 2023
   Programming Assignment 5
*/

class Track implements Comparable<Track> {    
	String src;
	String des;
	int cost;

	public Track(String src, String des, int cost) {
		this.src = src;
		this.des = des;
		this.cost = cost;
	}

	@Override
	public int compareTo(Track t) {
		if(t.cost == this.cost)  //if two city has the same cost then return the city alphabetically first
		{
			if(this.src.compareToIgnoreCase(t.src) > 0) 
				return this.cost;
			else 
				return t.cost;
		}
		
		return this.cost - t.cost;
	}
}

class DisjointSet {      
	Track[] parent;
	int n;
	int curIndex = 0;

	public DisjointSet(int n) {
		parent = new Track[n];
		this.n = n;
	}

	public void makeSet(String s1, String s2, int cost) {
		parent[curIndex++] = new Track(s1, s2, cost);
	}

	public String findRoot(String s, int count) {   
		if(count == 0)
			return s;

		String s1 = "";
		for (int l = 0; l < curIndex; l++) {
			if(s.equalsIgnoreCase(parent[l].des))
			{
				s1 = findRoot(parent[l].src, count-1);
				break;
			}
		}
		
		return s1 == ""?s:s1;
	}
	
	public String findEnd(String s, int count) {
		if(count == 0)
			return s;

		String s1 = "";
		for (int l = 0; l < curIndex; l++) {
			if(s.equalsIgnoreCase(parent[l].src))
			{
				s1 = findRoot(parent[l].des, count-1);
				break;
			}
		}
		
		return s1 == ""?s:s1;
	}
}

public class Railroad {
	int numOfTracks;
	PriorityQueue<Track> allTracks; // holds all the tracks but used for krustal algo which is getting the smallest every loop
	DisjointSet connectionTracks;  //holds the current tracks that we will output later

	public Railroad(int numOfTracks, String fileName) {
		this.numOfTracks = numOfTracks;
		allTracks = new PriorityQueue<>();
		connectionTracks = new DisjointSet(numOfTracks);

		read(fileName);

	}

//	public static void main(String[] args) {
//		Railroad r = new Railroad(4, "railroad1.txt");
//		
//		System.out.println(r.buildRailroad());
//	}

	public String buildRailroad() {
		String result = "";
		int cost = 0;
		int count = 0;    //number of the tracks that are currently in the disjoint set(connectionTrack)

		while (!allTracks.isEmpty()) {
            Track currentTrack = allTracks.poll();   //current track has the smallest cost of all the existing tracks
            
//            System.out.println("==" + currentTrack.src + " " + currentTrack.des + " " + + currentTrack.cost);
//            System.out.println(connectionTracks.findRoot(currentTrack.src, count) + " " + connectionTracks.findRoot(currentTrack.des, count) );
//            System.out.println(connectionTracks.findEnd(currentTrack.src, count) + " " + connectionTracks.findEnd(currentTrack.des, count) );

  
            //Check if the two city are already connected(can't have the same root or the same leave node)
            if (!connectionTracks.findRoot(currentTrack.src, count).equalsIgnoreCase(connectionTracks.findRoot(currentTrack.des, count)) && 
            	!connectionTracks.findEnd(currentTrack.des, count).equalsIgnoreCase(connectionTracks.findEnd(currentTrack.src, count)) ){
                
            	
				connectionTracks.makeSet(currentTrack.src, currentTrack.des, cost);
				
				if(currentTrack.src.compareToIgnoreCase(currentTrack.des) < 0)
					result = result + currentTrack.src + "---" + currentTrack.des + "	$" + currentTrack.cost + "\n";
				else 
					result = result + currentTrack.des + "---" + currentTrack.src + "	$" + currentTrack.cost + "\n";
				
    			cost += currentTrack.cost;
    			count++;
            }
        }
		
		result = result + "The cost of the railroad is $" + cost + ".";
		return result;
	}

	public void read(String fileName) {
		try {
			//File myFile = new File("src//" + fileName);
			File myFile = new File(fileName);
			
			Scanner myReader = new Scanner(myFile);
			while (myReader.hasNextLine()) {

				String line = myReader.nextLine();

				String[] parts = line.split(" ");
				String v1 = parts[0];
				String v2 = parts[1];
				int cost = Integer.parseInt(parts[2]);
				
				allTracks.add(new Track(v1, v2, cost));
			}
			myReader.close();
		} catch (FileNotFoundException e) {
			System.out.println("An error occurred:wrong file name or file not found");
		}
	}

}
