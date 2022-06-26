#!/bin/bash

#Bu komutu kullanarak dosyamızı executable hale çevirmiş olduk.
#chmod +x 181120_uyg1A.sh
# ./181120_uyg1A.sh

#Düzenli bir çıktı olarak gözükmesi için RUN #1 in saatini yazdırma işlemi yaptık.
saat=$(date +"%T")
echo -e "-------------------- RUN #1 ($saat) --------------------\n"


echo -e ">> 2.0: Make the script executable and add the corresponding commands to the top of the script as comment lines!"
sleep 1

echo -e ">> 2.1: creating directory (tempFiles) !!! if it is not already created !!!"
#mkdir komutuyla dosya açma işlemini yapıyoruz. -p komutu ise dosya var ise hata çıkmaması için kullanıldı.
mkdir -p tempFiles 
sleep 1

echo -e ">> 2.2: listing all content with subdirectories including sizes and other details"
#ls komutu ile listeleme yaptık. Bu listeleme için ayarlamalarımız şu şekildeydi. R: Subdirectories, r:reverse order, 1:one line, s: size information
ls -R1rs
sleep 1

echo -e "\n"
echo -e ">> 2.3: moving source code (.cpp && .sh extensions) to tempFiles folder"
#mv komutu ile dosya taşıma işlemi yaptık. *. yazdıktan sonra istediğimiz uzantıyı yazarak sadece bu dosyaların taşınmasını sağlıyoruz. En sona da directory olarak ./tempFiles veriyoruz.
mv *.cpp *.sh ./tempFiles ; ls -rR
#Değişen dosyaları gösterebilmek için yazdığımız ls -rR komutu
sleep 1

echo -e "\n"
echo -e ">> 2.4: removing non-folder content in working directory"
#Bu komut ile dosya tipine sahip olmayan file'ları silme işlemi yaptık.
find . -maxdepth 1 -type f  -delete ; ls -rR
#Değişen dosyaları gösterebilmek için yazdığımız ls -rR komutu
sleep 1

echo -e "\n"
echo -e ">> 2.5: copying all files in the tempFiles folder into working directory"
#cp komutu ile tempFiles'In içindeki tüm dosyaları (* koyarak tüm dosyaları seçiyoruz) bir önceki dosyaya (./) taşıma işlemi yaptık.
cp tempFiles/* ./ ; ls -rR
#Değişen dosyaları gösterebilmek için yazdığımız ls -rR komutu
sleep 1

echo -e "\n"
echo -e ">> 2.6: removing tempFiles folder with content"
#rm -rf ile tempFiles dosyasını tamamen sildik.
rm -rf ./tempFiles ; ls -rR
#Değişen dosyaları gösterebilmek için yazdığımız ls -rR komutu
sleep 1

echo -e "\n"
echo -e ">> 2.7: building the cpp code"
#g++ ile cpp kodumuzu derleyeceğimizi söyledik ve .cpp uzantılı dosya ismimizi girdik. -o ile output olarak ayarladık. Makine diline çevirilecek dosyayı da exe dosyası olarak yazdık.
g++ 181120_uyg1A.cpp -o 181120_uyg1A.exe ; ls -rR
#Değişen dosyaları gösterebilmek için yazdığımız ls -rR komutu
sleep 1

echo -e "\n"
echo -e ">> 2.8.0: Example run via console"
sleep 1

#i değişkenimizi 0 dan başlattık ve 4 olduğunda durmasını sağladık. Bu şekilde 3 kere döngü sağlanacak.
i=1
while [ $i -lt 4 ]
do
    echo -e ">> 2.8.$i: executing the builded cpp code, code output is directed to 181120_uyg1A_output.txt file and then the  character count is.."
    #Çıktımıza 0 ile 20 arasında değer verip çıktıyı da output.txt'ye yazdırma işlemi.
    ./181120_uyg1A.exe <<< $(($RANDOM%30)) >> 181120_uyg1A_output.txt
    echo -e "----" >> 181120_uyg1A_output.txt
    #wc ile txt dosyasından bilgiler alabiliyoruz. -w ile dosyadaki kelime sayısını bastırıyoruz. -m ile de toplam karakter sayısını bastırıyoruz. 
    wc -mw 181120_uyg1A_output.txt
    #i değişkenini her döngüde arttırma işlemi.
    i=`expr $i + 1`
    echo -e "\n"
    sleep 1
done

echo -e ">> 2.9: reading the 181120_uyg1A_output.txt file on terminal"
echo -e "181120_uyg1A_output.txt"
echo -e "-------------------------"
#cat ile txt dosyamızı terminale yazdırma işlemi yaptık.
cat 181120_uyg1A_output.txt
echo -e "-------------------------"

