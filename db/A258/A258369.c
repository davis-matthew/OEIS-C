/*
 * A258369  Stirling-Bernoulli transform of A027656.
 * https://oeis.org/A258369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k*k!*Stirling2(n+1,k+1)*A027656(k),
 *            where A027656(k) = k/2+1 for even k and 0 for odd k
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t len = n + 2;
    seq_t *r = mem_new(seq_t, (size_t)len);
    r[0] = 1; /* S2(0,0) */
    for (idx_t m = 1; m <= n + 1; m++) {
        for (idx_t j = m; j >= 1; j--) {
            r[j] = (seq_t)j * r[j] + r[j - 1];
        }
        r[0] = 0;
    }
    seq_t sum = 0;
    seq_t fact = 1; /* k! */
    for (idx_t k = 0; k <= n; k++) {
        if (k > 0) fact = fact * (seq_t)k;
        if ((k & 1ULL) == 0) {
            sum += fact * r[k + 1] * ((seq_t)k / 2 + 1);
        }
    }
    mem_free(r);
    return sum;
}
