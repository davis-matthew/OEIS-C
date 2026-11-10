/*
 * A369383  The smallest number k that can be partitioned in n ways as the sum
 *          of two numbers from A003601.
 * https://oeis.org/A369383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that #{(x,y): x<=y, x+y=k, x,y in A003601} =
 *            n; found by sieving sigma and tau and counting pair sums,
 *            doubling the limit until found
 * Range:     u8 n <= 53, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
	unsigned long long L = 64;
	for (;;) {
		unsigned long long *sg = (unsigned long long *)calloc(L + 1, sizeof(unsigned long long));
		unsigned int *tu = (unsigned int *)calloc(L + 1, sizeof(unsigned int));
		unsigned int *mem = (unsigned int *)malloc((L + 1) * sizeof(unsigned int));
		unsigned int *cnt = (unsigned int *)calloc(L + 1, sizeof(unsigned int));
		size_t m = 0;
		for (unsigned long long d = 1; d <= L; d++) {
			for (unsigned long long j = d; j <= L; j += d) {
				sg[j] += d;
				tu[j]++;
			}
		}
		for (unsigned long long j = 1; j <= L; j++) {
			if (sg[j] % tu[j] == 0) mem[m++] = (unsigned int)j;
		}
		for (size_t i = 0; i < m; i++) {
			for (size_t j = i; j < m; j++) {
				unsigned long long s = (unsigned long long)mem[i] + mem[j];
				if (s > L) break;
				cnt[s]++;
			}
		}
		unsigned long long found = 0;
		for (unsigned long long k = 1; k <= L; k++) {
			if ((idx_t)cnt[k] == n) { found = k; break; }
		}
		free(sg); free(tu); free(mem); free(cnt);
		if (found) return (seq_t)found;
		L *= 2;
	}
}
