/*
 * A340757  Counterexamples to a conjecture of Ramanujan about congruences
 *          related to the partition function.
 * https://oeis.org/A340757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 243 + 343 * T(n) where T(n) is defined by T(1) = 0, T(2)
 *            = 1, T(3) = 3, T(4) = 6, T(5) = 7, T(6) = 8 For n ≥ 7: let k = n
 *            - 6 let q = k / 4 let r = k % 4 T(n) = 8 + 7*q + S[r] and S[0] =
 *            0, S[1] = 2, S[2] = 5, S[3] = 6
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first 6 pre‑computed terms directly */
    static const seq_t first[6] = {
        243ULL,  /* a(1) */
        586ULL,  /* a(2) */
        1272ULL, /* a(3) */
        2301ULL, /* a(4) */
        2644ULL, /* a(5) */
        2987ULL  /* a(6) */
    };

    if (n <= 6) {
        return first[n - 1];
    }

    /* Compute the recursive part for n ≥ 7 */
    idx_t k = n - 6;
    idx_t q = k / 4;
    idx_t r = k % 4;

    static const idx_t add[4] = {0ULL, 2ULL, 5ULL, 6ULL};
    idx_t T = (idx_t)8 + (idx_t)7 * q + add[r];

    /* Return the final value: a(n) = 243 + 343 * T */
    return (seq_t)243 + (seq_t)343 * (seq_t)T;
}
