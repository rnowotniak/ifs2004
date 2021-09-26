

for p in *;
do 
	echo $[n++];
	cp $p mini-$p;
	mogrify -strip +antialias +dither -thumbnail 250 mini-$p;
done

