for file in testfilesB/*; do
    filename="$(basename "$file")"
    if [ ${file: -7} == ".cities" ]
    then

        filename_without_extension="${filename%.*}"
        echo FICHEIRO: $filename_without_extension

        ./tuktuk testfilesB/$filename
        diff testfilesB/$filename_without_extension.walks testfilesB_walks/$filename_without_extension.walks;
    fi
done
