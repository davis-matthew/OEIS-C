/*
 * A273926  Given G(x) such that G( G(x)^2 - G(x)^3 ) = x^2, then G(x) =
 *          Sum_{n>=1} A273925(n)*x^n / 2^a(n).
 * https://oeis.org/A273926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n ≥ 1, if n is odd (n = 2k – 1): a(n) = 4·(k – 1) –
 *            popcount(k – 1) if n is even (n = 2k): a(n) = k + Σ_{1 ≤ i < k,
 *            i even} popcount_lt2(i) where popcount(x) = number of 1‑bits in
 *            x and popcount_lt2(i) = trailing zero count of i (tzc(i) = #{ j
 *            ≥ 0 | 2^j divides i })
 * Range:     u8 n <= 130, u16 n <= 261, u32 n <= 261, u64 n <= 261,
 *            u128 n <= 261, big any n
 * Helpers:   tzc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/tzc.h"

/* popcount for 64‑bit values */
static inline idx_t popcnt(idx_t x)
{
    return __builtin_popcountll(x);
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* odd indices: n = 2k – 1 */
    if ((n & 1ULL) == 1ULL) {
        idx_t k = (n + 1) >> 1;        /* k = (n+1)/2          */
        idx_t m = k - 1;                /* m = k – 1            */
        return (seq_t)(4 * m - popcnt(m));
    }

    /* even indices: n = 2k */
    idx_t k = n >> 1;                    /* k = n/2              */
    seq_t sum = 0;
    for (idx_t i = 2; i < k; i += 2) {   /* iterate even i < k   */
        sum += (seq_t)tzc(i);
    }
    return (seq_t)(k + sum);
}
