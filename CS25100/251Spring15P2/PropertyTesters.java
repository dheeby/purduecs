public class PropertyTesters {
	
	public static boolean testForHeapness (Song[] songs) {
		/* TODO write code to test if the 'songs' array is a valid heap */
		for (int i = 0; i <= (songs.length - 2) / 2; i++) {
			int x = songs[i].getNumberOfTimesPlayedSoFar();
			int y = songs[2*i + 1].getNumberOfTimesPlayedSoFar();
			int z = 0;
			if ((2*i + 2) >= songs.length) {
				z = -2;
			} else {
				z = songs[2*i + 2].getNumberOfTimesPlayedSoFar();
			}
			
			if (!((x >= y) && (x >= z))) {
				return false;
			}
		}
		return true;
	}
	
	public static boolean testDataConsistency (SongsRecord record) {
		int numberOfSongs = record.getNumberOfSongs();
		Song[] songsHeap = record.getHeapOfSongs();
		int[] songPositions = record.getPositionsOfSongs();
		int[] songsCounts = record.getSongPlayedCounts();
		
		boolean isConsistent = true;

		for(int i = 0; i < numberOfSongs && isConsistent; ++i) {
			isConsistent = isConsistent && (songsHeap[songPositions[i]].getNumberOfTimesPlayedSoFar() == songsCounts[i]); 
			isConsistent = isConsistent && (songPositions[songsHeap[i].getSongID()] == i);
		}

		return isConsistent;
	}
}
