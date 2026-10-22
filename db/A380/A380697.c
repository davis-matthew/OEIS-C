/*
 * A380697  Frobenius number of the set S = {e_i+2; 1 <= i <= m}, where the
 *          e_i are the exponents in the binary expansion n = Sum_{i=1..m}
 *          2^e_i, or 0 if GCD(S) = A326674(2*n) > 1.
 * https://oeis.org/A380697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A380697 – Frobenius number of the set S = {e_i+2; 1 ≤ i ≤ m},
 * where the e_i are the exponents in the binary expansion of n.
 * 
 * The implementation follows the definition exactly:
 *   1. Extract all exponents  e_i  from n.
 *   2. Build the set S = {e_i + 2}.
 *   3. If gcd(S) > 1 the Frobenius number is defined to be 0.
 *   4. For |S| = 1  → 0.
 *   5. For |S| = 2  →  a·b – a – b  (two‑coin formula).
 *   6. For |S| ≥ 3  →  dynamic programming up to the bound
 *       min(S)·max(S) – min(S) – max(S).
 */
 
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Extract the exponents e_i (bit positions where n has a 1). */
    seq_t s[64];
    size_t m = 0;
    idx_t t = n;
    for (idx_t e = 0; t; ++e) {
        if (t & 1U) {
            s[m++] = (seq_t)(e + 2U);
        }
        t >>= 1;
    }

    /* Zero elements should never happen for n ≥ 1, but be defensive. */
    if (m == 0)
        return (seq_t)0;

    /* Compute the GCD of all elements in S. */
    seq_t g = s[0];
    for (size_t i = 1; i < m; ++i)
        g = gcd(g, s[i]);

    /* If the GCD is > 1 the Frobenius number is defined to be 0. */
    if (g > (seq_t)1)
        return (seq_t)0;

    /* a(1) … a(3) are handled automatically by the rules below. */
    if (m == 1)
        return (seq_t)0;

    if (m == 2) {
        seq_t a0 = s[0], a1 = s[1];
        return a0 * a1 - a0 - a1;
    }

    /* m ≥ 3: use dynamic programming up to an upper bound. */
    seq_t min_coin = s[0] < s[1] ? s[0] : s[1];
    seq_t max_coin = s[0] > s[1] ? s[0] : s[1];
    for (size_t i = 2; i < m; ++i) {
        if (s[i] < min_coin)
            min_coin = s[i];
        if (s[i] > max_coin)
            max_coin = s[i];
    }

    /* Upper bound known to be >= Frobenius number. */
    idx_t limit = (idx_t)((unsigned long long)min_coin * (unsigned long long)max_coin
                         - min_coin - max_coin);

    bool *reach = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!reach)
        return (seq_t)0; /* unlikely, but defensive */

    for (idx_t i = 0; i <= limit; ++i)
        reach[i] = false;
    reach[0] = true;

    for (size_t i = 0; i < m; ++i) {
        seq_t coin = s[i];
        if ((seq_t)coin > (seq_t)limit)
            continue;
        for (idx_t sum = (idx_t)coin; sum <= limit; ++sum) {
            if (reach[sum - (idx_t)coin])
                reach[sum] = true;
        }
    }

    seq_t last_nonrep = (seq_t)0;
    for (idx_t i = limit; ; --i) {
        if (!reach[i]) {
            last_nonrep = (seq_t)i;
            break;
        }
        if (i == 0)     /* avoid unsigned underflow */
            break;
    }

    free(reach);
    return last_nonrep;
}
