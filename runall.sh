# Run all input combinations

for DIFF in 0.100 0.010 0.001; do
	for d in 0.25 0.45 0.65 0.85; do
		./bin/pagerank $DIFF $d
	done
done
