/*
 * A331714  Number of non-isomorphic set-systems with 3 sets each with n
 *          elements.
 * https://oeis.org/A331714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Σ_{w=0}^{n} f(w) – (n+1) * * f(2k) = (k+1)(k+2)(2k+3)/6 *
 *            f(2k+1) = (k+1)(k+2)(k+3)/3 * * The term Σ_{w=0}^{n} f(w) counts
 *            all unlabeled 3‑set systems * (allowing equal columns). Exactly
 *            (n+1) of those are * degenerate (at least two columns
 *            identical). Subtracting * that yields the desired...
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;

    for (idx_t w = 0; w <= n; ++w)
    {
        idx_t k = w >> 1;           /* k = floor(w/2)   */
        if ((w & 1) == 0)           /* w even : use f(2k)   */
        {
            seq_t a = (seq_t)k + 1;
            seq_t b = (seq_t)k + 2;
            seq_t c = (seq_t)k * 2 + 3;
            total += (a * b * c) / 6;
        }
        else                        /* w odd : use f(2k+1)  */
        {
            seq_t a = (seq_t)k + 1;
            seq_t b = (seq_t)k + 2;
            seq_t c = (seq_t)k + 3;
            total += (a * b * c) / 3;
        }
    }
    return total - ((seq_t)n + 1);
}
