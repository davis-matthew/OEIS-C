/*
 * A367707  Number of degree 6 vertices in the n-Menger sponge graph.
 * https://oeis.org/A367707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2/17)*20^n - (6/5)*8^n + (432/85)*3^n - 8 Recurrence
 *            (valid for n ≥ 1): a(1) = 0 a(n) = 20*a(n-1) + ((9*8^n + 760) -
 *            144*3^n)/5 (n ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n == 1) return (seq_t)0;

    /* Initialize powers: pow8 = 8^1, pow3 = 3^1 */
    seq_t pow8 = (seq_t)8;
    seq_t pow3 = (seq_t)3;

    seq_t a_val = (seq_t)0; /* value of a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* Update powers: pow8 = 8^i, pow3 = 3^i */
        pow8 = mul_small(pow8, (unsigned)8);  /* pow8 *= 8 */
        pow3 = mul_small(pow3, (unsigned)3);  /* pow3 *= 3 */

        /* Compute N = 9*pow8 + 760 - 144*pow3  (N is guaranteed non‑negative) */
        seq_t t1 = mul_small(pow8, (unsigned)9);      /* 9*pow8 */
        seq_t t2 = t1 + ((seq_t)760);                 /* +760 */
        seq_t t3 = mul_small(pow3, (unsigned)144);    /* 144*pow3 */
        seq_t N   = t2 - t3;                          /* := (9*pow8 +760) - 144*pow3 */

        /* term = N / 5 */
        seq_t term = N / ((seq_t)5);

        /* Update a_val: a(n) = 20*a(n-1) + term */
        a_val = (seq_t)20 * a_val + term;
    }

    return a_val;
}
