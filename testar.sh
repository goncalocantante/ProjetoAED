for file in testfiles/*; do
    filename="$(basename "$file")"
    if [ ${file: -7} == ".cities" ]
    then
        
        filename_without_extension="${filename%.*}"
        echo FICHEIRO: $filename_without_extension

        ./tuktuk testfiles/$filename
        diff testfiles/$filename_without_extension.walks testfiles_walks/$filename_without_extension.walks;
    fi
done
