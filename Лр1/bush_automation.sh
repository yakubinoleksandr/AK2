
FILE="numbers.txt"

> "$FILE"

for _ in {1..5}; do
    num=$(( (RANDOM % 100) + 1 ))
    echo "$num" >> "$FILE"
done

echo "Згенеровані числа у файлі:"
cat "$FILE"

min_num=$(sort -n "$FILE" | head -n 1)

echo "$min_num" > "$FILE"

echo -e "\nРезультат після видалення (залишилось найменше):"
cat "$FILE"
