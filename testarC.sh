for file in testfilesC/*; do
    filename="$(basename "$file")"
    if [ ${file: -7} == ".cities" ]
    then
        
        filename_without_extension="${filename%.*}"
        echo FICHEIRO: $filename_without_extension

        ./tuktuk testfilesC/$filename
        diff testfilesC/$filename_without_extension.walks testfilesC_walks/$filename_without_extension.walks;
    fi
done