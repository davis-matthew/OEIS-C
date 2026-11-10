/*
 * A320250  Expansion of Product_{k>=1} (1 + x^k)^(A048272(k)).
 * https://oeis.org/A320250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Product_{k>=1} (1+x^k)^c(k) with c(k) = A048272(k) = (#odd
 *            divisors of k) - (#even divisors of k); a(n) = [x^n]. Computed
 *            by multiplying/dividing the series by (1+x^k) using only
 *            additions/subtractions
 * Range:     u8 n <= 44, u16 n <= 115, u32 n <= 347, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 908 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    int *c = (int *)malloc((N + 1) * sizeof(int));
    for (size_t i = 0; i <= N; i++) c[i] = 0;
    for (size_t d = 1; d <= N; d++) {
        int s = (d & 1) ? 1 : -1;
        for (size_t m = d; m <= N; m += d) c[m] += s;
    }
    seq_t *f = mem_new(seq_t, N + 1);
    f[0] = 1;
    for (size_t k = 1; k <= N; k++) {
        int e = c[k];
        while (e > 0) {
            for (size_t i = N; i >= k; i--) f[i] += f[i - k];
            e--;
        }
        while (e < 0) {
            for (size_t i = k; i <= N; i++) f[i] -= f[i - k];
            e++;
        }
    }
    seq_t r = f[N];
    mem_free(f);
    free(c);
    return r;
}
