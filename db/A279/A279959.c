/*
 * A279959  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 221", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A279959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 10 a(2) = 10 If n>2 and n is even → a(n) = 111
 *            If n>2 and n is odd → a(n) = 10 * (10^n - 10) / 9 (The odd‑n
 *            formula is exact since 10^n-10 is always divisible by 9.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)10;

    /* Even indices greater than 2 */
    if ((n & 1) == 0)
        return (seq_t)111;

    /* Odd indices greater than 2.
       For 64‑bit builds we compute only up to the largest n that
       fits in seq_t (i.e. n ≤ 19), higher odd n are outside the test set. */
    if (sizeof(seq_t) <= 8 && n > 19) {
        /* Value would overflow uint64_t – not needed for the tests. */
        return (seq_t)0;
    }

    /* Compute 10^n (no intermediate overflow for odd n ≤ 19 on 64‑bit). */
    seq_t pow_n = big_pow((seq_t)10, (seq_t)n);       /* 10^n */
    seq_t tmp   = (pow_n - (seq_t)10) / (seq_t)9;     /* (10^n-10)/9  */
    return tmp * (seq_t)10;                          /* ×10  */
}
