/*
 * A369152  Total number of digits in row n of Pascal's triangle.
 * https://oeis.org/A369152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + n + Σ_{k=0..n} ⌊log10(C(n,k))⌋ where C(n,k)=n choose
 *            k
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 30, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   binom, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helper functions for big integer binomial computation
   and decimal digit counting. */
#include "../utils/binom.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* total number of decimal digits in row n of Pascal's triangle */
    seq_t total = 0;
    seq_t cur = 1;                     /* C(n,0) = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* add digits of current binomial coefficient */
        int d = digit_len(cur);
        total += d;

        /* compute next coefficient C(n,k+1) if needed */
        if (k < n) {
            /* cur = cur * (n - k) / (k + 1) */
            cur = (cur * (seq_t)(n - k)) / (seq_t)(k + 1);
        }
    }
    return total;
}
