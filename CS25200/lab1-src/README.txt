Neil Allison (nalliso)

pwcheck.sh
	- Working Features
		- Checking invalid password length
		- Points addition for password length
		- Points addition for special characters
		- Points addition for at least one number
		- Points addition for at least one alpha character
		- Points deduction for alphanumeric consecutive repetition
		- Points deduction for consecutive lowercase characters
		- Points deduction for consecutive uppercase characters
		- Points deduction for consecutive numbers
	- Non-working Features
		- None (that I know of)
backup.sh
	- Working Features
		- Initial copy into backupdir
		- Sleep for interval-secs
		- Diffing files and saving output
		- Checking for difference
			- Copy file into backupdir
			- Send email with diff output
	- Non-working Features
		- None (that I know of)
	- Extra Features
		- Prompt for missing parameters
		- Prompt for invalid file
		- Prompt for invalid backupdir
		- Naming scheme for date and time (YYYY-MM-DD-hh-mm-ss)
		- Delete oldest file when max-backups reached
