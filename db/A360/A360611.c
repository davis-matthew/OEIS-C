/*
 * A360611  Expansion of Sum_{k>=0} (k * x * (1 + x))^k.
 * https://oeis.org/A360611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-k)^(n-k) * binomial(n-k,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 376 (2026-11-03)
 * Helpers:   binomial, ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Cache of precomputed powers i^i */
    static seq_t *pows = NULL;
    static size_t pows_cap = 0;          /* number of precomputed entries */

    /* Expand cache if necessary */
    if ((size_t)n + 1 > pows_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_pows = mem_resize(seq_t, pows, new_cap);
        if (!new_pows) return 0;        /* out of memory – return 0 as a failure value */

        /* Compute new entries */
        for (size_t i = pows_cap; i <= (size_t)n; ++i) {
            if (i == 0) {
                new_pows[i] = (seq_t)1;          /* convention 0^0 = 1 for a(0) */
            } else {
                new_pows[i] = ipow((seq_t)i, (seq_t)i);
            }
        }
        pows = new_pows;
        pows_cap = new_cap;
    }

    seq_t sum = 0;
    idx_t half = n / 2;           /* floor(n/2) */

    for (idx_t k = 0; k <= half; ++k) {
        idx_t m   = n - k;                     /* term uses (n-k)^(n-k)  */
        seq_t term = pows[(size_t)m] * binomial((idx_t)m, k);
        sum += term;
    }
    return sum;
}
