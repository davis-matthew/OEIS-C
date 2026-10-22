/*
 * A310439  Coordination sequence Gal.6.321.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((24*n + k(n))/5) where k(n) is periodic (period 10)
 *            with k(0) = 5 and for n>=1, k(n) = [-4, 2, 3, -1, 0, 1, -3, -2,
 *            4, 0] based on n mod 10. This reproduces the OEIS sequence
 *            A310439
 * Range:     u8 n <= 10, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(0) = 1.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for offset 0 */
    if (n == 0)
        return 1;

    /* k(n) table for n >= 1 (period 10) */
    static const int8_t k_table[10] = {
        -4,  /* n ≡ 1 (mod 10) */
         2,  /* n ≡ 2 */
         3,  /* n ≡ 3 */
        -1,  /* n ≡ 4 */
         0,  /* n ≡ 5 */
         1,  /* n ≡ 6 */
        -3,  /* n ≡ 7 */
        -2,  /* n ≡ 8 */
         4,  /* n ≡ 9 */
         0   /* n ≡ 0 (mod 10), n≥10 */
    };

    /* Compute 24*n as seq_t to avoid intermediate overflow */
    seq_t prod = (seq_t)n * 24ULL;       /* 24 is small enough to stay in seq_t */

    /* Base quotient and remainder when dividing by 5 */
    seq_t q = prod / 5;
    int32_t rem = (int32_t)(prod % 5);    /* remainder is 0..4, safe to cast */

    /* k based on (n-1) mod 10 */
    int32_t k = k_table[(n - 1) % 10];

    /* Adjust quotient based on the small offset k */
    int32_t adj = rem + k;
    if (adj >= 5)
        ++q;
    else if (adj < 0)
        --q;

    return q;
}
