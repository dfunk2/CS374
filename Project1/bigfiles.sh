ls -la /usr/local/bin 
echo ====
ls -la /usr/local/bin | tr -s " " | cut -d' ' -f 5,9 | sort -n -r   
echo ====
