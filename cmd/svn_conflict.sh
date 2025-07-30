# svn up冲突时会有多个文件，如1.txt.rNew、1.txt.mine、1.txt.merged等，这些文件是冲突的各个版本，需要手动选择一个作为最后提交的版本。

# 使⽤1.txt.rNew作为最后提交的版本，也就是使用远程的版本
svn resolve --accept theirs-full 1.txt

# 使⽤1.txt.mine作为最后提交的版本，也就是全部使用自己的代码
svn resolve --accept mine-full 1.txt

# 使⽤1.txt.mine的冲突部分作为最后提交的版本
svn resolve --accept mine-conflict 1.txt

# 使⽤1.txt.rNew的冲突部分作为最后提交的版本
svn resolve --accept theirs-conflict 1.txt