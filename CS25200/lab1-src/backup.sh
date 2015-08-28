#!/bin/bash
#check if there are not 4 arguments
if [ $# -ne 4 ] ; then
	echo Usage: backup\.sh \<file\> \<backupdir\> \<interval-secs\> \<max-backups\>
	exit 1
fi

#check if file does not exist
if [ ! -e $1 ] ; then
	echo File does not exist\.
	exit 1
fi

#check if backupdir does not exist
if [ ! -d $2 ] ; then
	echo Backup directory does not exist\.
	exit 1
fi

#copy file into backupdir
CURRENTNAME=$2/`date +%Y-%m-%d-%H-%M-%S`-$1
cp $1 $CURRENTNAME

#initialize counter for backup files
#count any files already present in backupdir
BACKUPCOUNT=0
for f in $(ls $2) ; do
	let BACKUPCOUNT=BACKUPCOUNT+1
done

#main loop
while [ 1 ] ; do	
	#Sleep for interval-secs
	sleep $3
	#diff the two files and save output to file and variable
	DIFFSTRING=`diff $1 $CURRENTNAME`
	#if they are different
	if [ ${#DIFFSTRING} -gt 0 ] ; then
		#check if backup limit reached
		if [ $BACKUPCOUNT -ge $4 ] ; then
			#delete oldest backup
			rm $2/`ls $2 | head -1`
			let BACKUPCOUNT=BACKUPCOUNT-1
		fi
		#copy file into backupdir
		CURRENTNAME=$2/`date +%Y-%m-%d-%H-%M-%S`-$1
		cp $1 $CURRENTNAME
		let BACKUPCOUNT=BACKUPCOUNT+1
		#send email with out from diff
		echo $DIFFSTRING | /usr/bin/mailx -s "Backup Diff" $USER
	fi
done
