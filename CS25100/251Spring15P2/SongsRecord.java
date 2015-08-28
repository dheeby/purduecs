
public class SongsRecord {
	private int nSongs;
	
	private Song[] songsHeap;
	private int[] songsCount;
	private int[] songsPosition;

	public SongsRecord(int nSongs) {
		this.nSongs = nSongs;
		
		// H[P[x]]
		this.songsHeap = new Song[nSongs];
		// T[x]
		this.songsCount = new int[nSongs];
		// P[x]
		this.songsPosition = new int[nSongs];
	}
		
	public void addSongEntry(int songID, int numberOfTimesPlayed) {
		this.songsHeap[songID] = new Song(songID, numberOfTimesPlayed);
	}
	
	public Song[] getHeapOfSongs() {
		return this.songsHeap;
	}
	
	public int[] getPositionsOfSongs() {
		return this.songsPosition;
	}
	
	public int[] getSongPlayedCounts() {
		return this.songsCount;
	}
	
	public int getNumberOfSongs() {
		return this.nSongs;
	}

	public void process() {
		/* TODO Write code to:
			(1) Turn 'songsHeap' into a valid heap
			(2) Populate 'songsCount' and 'songsPosition' correctly */

		while (!PropertyTesters.testForHeapness(songsHeap)) {
			for (int i = 0; i <= (songsHeap.length - 2) / 2; i++) {
				int x = songsHeap[i].getNumberOfTimesPlayedSoFar();
				int y = songsHeap[2*i + 1].getNumberOfTimesPlayedSoFar();
				int z = 0;

				// Check for when last node in heap is a left or right child of parent
				if ((2*i + 2) >= songsHeap.length) {
					z = -2;
				} else {
					z = songsHeap[2*i + 2].getNumberOfTimesPlayedSoFar();
				}

				// Check if left child is greater than parent 
				if (y > x) {
					Song tempSong = songsHeap[i];
					songsHeap[i] = songsHeap[2*i + 1];
					songsHeap[2*i + 1] = tempSong;

					songsPosition[i] = 2*i + 1;
					songsPosition[2*i + 1] = i;

					x = songsHeap[i].getNumberOfTimesPlayedSoFar();
				}
				// Check if right child is greater than parent
				if ((z != -2) && (z > x)) {
					Song tempSong = songsHeap[i];
					songsHeap[i] = songsHeap[2*i + 2];
					songsHeap[2*i + 2] = tempSong;

					songsPosition[i] = 2*i + 2;
					songsPosition[2*i + 2] = i;
				}
			}
		}

		for (int i = 0; i < nSongs; i++) {
			int x = songsHeap[i].getSongID();
			songsPosition[x] = x;
			songsCount[x] = songsHeap[i].getNumberOfTimesPlayedSoFar();
		}

		System.out.println("HEAP AFTER: ");
		for (int i = 0; i < nSongs; i++) {
				System.out.println("Play Count: " + songsHeap[i].getNumberOfTimesPlayedSoFar());
		}

		for (int i = 0; i < nSongs; i++) {
				System.out.println("Count: " + songsCount[i] + "\tPosition: " + songsPosition[i]);
		}
	}
	
	public void addSongOccurrence(int songID) {
		/* TODO Write code to:
			(1) Read 'songsPosition' and get the position of the song in the heap. Then increase the count by 1. After increasing the count by 1, move the song up if required and ensure that the heap invariant is maintained.
			(2) Modify 'songsPosition' and 'songsCount' accordingly to ensure that everything is consistent.
		*/
	}

	public String getTopTwo() {
		String output = "";
		
		// Get top song
		output = Integer.toString(songsHeap[0].getSongID());
		output += " (" + Integer.toString(songsHeap[0].getNumberOfTimesPlayedSoFar())  + ")";
		
		// Get runner up
		int runnerUp = 2;
		if(songsHeap[1].getNumberOfTimesPlayedSoFar() > songsHeap[2].getNumberOfTimesPlayedSoFar() ||
				(songsHeap[1].getNumberOfTimesPlayedSoFar() == songsHeap[2].getNumberOfTimesPlayedSoFar() 
				&& songsHeap[1].getSongID() < songsHeap[2].getSongID()))
			runnerUp = 1;
		output += "; " + Integer.toString(songsHeap[runnerUp].getSongID());
		output += " (" + Integer.toString(songsHeap[runnerUp].getNumberOfTimesPlayedSoFar())  + ")";

		return output;
	}
}
