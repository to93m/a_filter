#いちいちうつのめんどくさいから省略しようよ的な
#ruby raw2wav.rb ファイルの場所 変換したいファイルの.raw なし
name = ARGV[0].split(".")
$command = "sox -r 16k -e signed -b 16 -c 1 \"#{name[0]}.raw\" \"./#{name[0]}.wav\""
system($command)
