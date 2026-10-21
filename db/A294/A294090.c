/*
 * A294090  Base-10 complementary numbers: n equals the product of the 10's
 *          complement of its digits.
 * https://oeis.org/A294090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product of (10 - digit) over all decimal digits of a(n)
 *            Recurrence: a(n + 4) = 10 * a(n) for all n >= 3 Initial values:
 *            a(1) = 5, a(2) = 18, a(3) = 35, a(4) = 50
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 35, u64 n <= 73, u128 n <= 150,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* handle the two special indices directly */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)18;

    /* indices ≥ 3 use the 4‑cycle recurrence */
    idx_t shift = (n - 3) / 4;                  /* number of times 10· is applied */
    idx_t rem   = (n - 3) % 4;                  /* 0 → 35, 1 → 50, 2 → 180, 3 → 315 */

    static const seq_t base[4] = {
        (seq_t)35,   /* rem = 0 → a(3) */
        (seq_t)50,   /* rem = 1 → a(4) */
        (seq_t)180,  /* rem = 2 → a(5) */
        (seq_t)315   /* rem = 3 → a(6) */
    };

    seq_t pow10 = seq_pow((seq_t)10, shift);    /* 10^shift */
    return base[rem] * pow10;
}
