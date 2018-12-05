for file in testfilesA/*; do
    filename="$(basename "$file")"
    if [ ${file: -7} == ".cities" ]
    then

        filename_without_extension="${filename%.*}"
        echo FICHEIRO: $filename_without_extension

        ./tuktuk testfilesA/$filename
        diff testfilesA/$filename_without_extension.walks testfilesA_walks/$filename_without_extension.walks;
    fi
done