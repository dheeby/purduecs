import java.util.Scanner;

public class HitSongs {
	
	public static void main(String args[]) {
		Scanner in = new Scanner(System.in);

		int nCases = in.nextInt();
		in.nextLine();

		for(int i = 0; i < nCases; ++i) {
			if(i > 0)
				System.out.println("\n");
			System.out.println("~Test Case " + (i + 1) + "~");

			int nSongs = in.nextInt();
			in.nextLine();

			SongsRecord record = new SongsRecord(nSongs);
						
			for(int j = 0; j < nSongs; ++j) {
				int songID = in.nextInt();
				int numberOfTimesPlayed = in.nextInt();
				in.nextLine();
				record.addSongEntry(songID, numberOfTimesPlayed);
			}
			
			record.process();
						
			assert PropertyTesters.testForHeapness(record.getHeapOfSongs());
			assert PropertyTesters.testDataConsistency(record);

			int nUpdates = in.nextInt();
			in.nextLine();
			System.out.println("Number of updates: " + nUpdates);
			
			int instance = 0;
			for(int j = 0; j < nUpdates; ++j) {
				int songNumber = in.nextInt();
				if(songNumber == -1) {
					String topTwo = record.getTopTwo();
					System.out.println("Top Two Songs for query " + (++instance) + ": " + topTwo);
				} else {
					record.addSongOccurrence(songNumber);
				}
			}

			assert PropertyTesters.testForHeapness(record.getHeapOfSongs());
			assert PropertyTesters.testDataConsistency(record);
		}
		in.close();
	}
}