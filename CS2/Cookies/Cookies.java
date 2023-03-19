/* Feifan Cao
 * Dr. Steinberg
 * COP3503 Spring 2023
 * Programming Assignment 3
 */

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.Arrays;

public class Cookies {
	int[] greedyFactor;
	int[] sizeFactor;
	int happyCamper;
	int angryCamper;

	public Cookies(int numCookies, int numCampers, String campers, String cookies) {
		greedyFactor = new int[numCampers];
		sizeFactor = new int[numCookies];

		read(campers, cookies);
	}

	public void passCookies() {
		Arrays.sort(greedyFactor);
		Arrays.sort(sizeFactor);

		int l = 0; // index for cookies
		int k = 0; // index for damn kids
		while(k < greedyFactor.length && l < sizeFactor.length) { 
			if (greedyFactor[k] <= sizeFactor[l]) {
				happyCamper++;
				l++;
				k++;
			}else {
				l++;
			}
		}
		
		angryCamper = greedyFactor.length - happyCamper;


	}

	public void display() {
		System.out.println("There are " + happyCamper + " happy campers. ");
		System.out.println("There are " + angryCamper + " angry campers. ");
	}

	public void read(String g, String s) {
		try {
			File greedyFile = new File("src//" + g);
			File sizeFile = new File("src//" + s);

			// File greedyFile = new File(g);
			// File sizeFile = new File(s);

			Scanner myReaderg = new Scanner(greedyFile);
			Scanner myReaders = new Scanner(sizeFile);

			int i = 0;
			int j = 0;

			while (myReaderg.hasNextLine() && myReaders.hasNextLine()) {

				String lineg = myReaderg.nextLine();
				String lines = myReaders.nextLine();

				greedyFactor[i++] = Integer.parseInt(lineg);
				sizeFactor[j++] = Integer.parseInt(lines);
			}

			while (myReaderg.hasNextLine()) {
				String lineg = myReaderg.nextLine();
				greedyFactor[i++] = Integer.parseInt(lineg);
			}

			while (myReaders.hasNextLine()) {
				String lines = myReaders.nextLine();
				sizeFactor[j++] = Integer.parseInt(lines);
			}

			myReaderg.close();
			myReaders.close();

		} catch (FileNotFoundException e) {
			System.out.println("An error occurred:wrong file name or file not found");
		}
	}

//	public static void main(String[] args) {
//		Cookies cookies = new Cookies(105, 100, "campers1.txt", "cookies1.txt");
//		cookies.passCookies();
//		cookies.display();
//	}

}
