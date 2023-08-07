/* Name: Feifan Cao
 * Dr. Andrew Steinberg
 * COP3503 Spring 2023
 * Programming Assignment 4
 */

public class SequenceAlignment {

	private int diffVowel = 1;
	private int diffConstant = 1;
	private int gap = 2;
	private int diffVowelConstant = 3;
	private int[][] arr;
	private int l1;
	private int l2;
	private String s1;
	private String s2;

	public String getAlignment() {
		int bigLength = s1.length() + s2.length();  // if no letter matches between 2 strings then it's both length
				
		char[] newS1 = new char[bigLength];   //new string stored in an array
		char[] newS2 = new char[bigLength];   //new string stored in an array
				
		int i = l1 - 1;  //arr index 
		int j = l2 - 1;  //arr index
		
		int si1 = bigLength - 1;	//new string 1 index from the end
		int si2 = bigLength - 1;    //new string 2 index from the end
		
		int alpha;
		
		while(i != 0 && j != 0) {
			char curCh1 = s1.charAt(i - 1);    //current character of the input string1 at index i 
			char curCh2 = s2.charAt(j - 1);		//current character of the input string2 at index j
					
			//get the alpha value
			alpha = getAlpha(curCh1, curCh2);
						
			//traverse backwards to see if the path matching up
			if (arr[i][j] == arr[i-1][j-1] + alpha) {
				newS1[si1--] = curCh1;
	            newS2[si2--] = curCh2;
	            i--; j--;
			}else if (arr[i - 1][j] + gap == arr[i][j])
	        {
	            newS1[si1--] = curCh1;
	            newS2[si2--] = '-';
	            i--;
	        }
	        else
	        {
	            newS1[si1--] = '-';
	            newS2[si2--] = curCh2;
	            j--;
	        }
		}
		
//		for (int k = 0; k < newS1.length; k++) {
//			if(newS1[k] != '\u0000') System.out.print(k);
//			else System.out.print(k);
//		}
		
		return new String(newS1, si1+1, bigLength-si1-1) + " " + new String(newS2, si2+1, bigLength - si2-1);
	}
	
	public boolean isVowel(char c) {
		if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') return true;
		
		return false;
	}
	
	public int getAlpha(char curCh1, char curCh2)
	{
		if(curCh1 == curCh2)	
		{
			return 0;
		}else if(isVowel(curCh1) && isVowel(curCh2)) {
			return diffVowel;
		}else if(!isVowel(curCh1) && !isVowel(curCh2))
		{
			return diffConstant;
		}else {
			return diffVowelConstant;
		}
	 }

	public int computeAlignment(int n) {
		int alpha;
		
		//tabulation
		for (int j = 1; j < l2; j++) {
			
			char curCh2 = s2.charAt(j-1);
			
			for (int i = 1; i < l1; i++) {
				
				char curCh1 = s1.charAt(i-1);
								
				alpha = getAlpha(curCh1, curCh2);
								
				arr[i][j] = Math.min(alpha + arr[i-1][j-1], Math.min(gap + arr[i-1][j], gap + arr[i][j-1]));
			}
		}
		
		return arr[l1-1][l2-1];
	}

	public SequenceAlignment(String s1, String s2) {
		this.s1 = s1;
		this.s2 = s2;
		
		l1 = s1.length() + 1;   //input string 1 length
		l2 = s2.length() + 1;	//input string 2 length
		
		arr = new int[l1][l2];
		
		//initial initialization
		for (int i = 0; i < l1; i++) {
			arr[i][0] = i * gap;
		}
		
		for (int j = 0; j < l2; j++) {
			arr[0][j] = j * gap;
		}
	}
}
