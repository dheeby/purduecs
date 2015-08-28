
public class Song {
	private int songID;
	
	private int numberOfTimesPlayed;
	
	public Song(int songID) {
		this(songID, 0);
	}

	public Song(int songID, int numberOfTimesPlayed) {
		this.songID = songID;
		this.numberOfTimesPlayed = numberOfTimesPlayed;
	}
	
	public void increasePlayedCountByOne() {
		++this.numberOfTimesPlayed;
	}
	
	public int getSongID() {
		return this.songID;
	}
	
	public int getNumberOfTimesPlayedSoFar() {
		return this.numberOfTimesPlayed;
	}
}
