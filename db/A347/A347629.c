/*
 * A347629  Number of minimum dominating sets in the n-pan graph (for n > 2).
 * https://oeis.org/A347629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n%3 == 0; * (n^2 + 7*n + 28) / 18 if n%3 == 1; * (n
 *            + 1) / 3 if n%3 == 2
 * Range:     u8 n <= 12, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t rmod = n % 3;
    seq_t result;
    if (rmod == 0) {
        result = 1;
    } else if (rmod == 1) {
        seq_t nn = (seq_t)n;
        seq_t val = nn * nn + (seq_t)7 * nn + (seq_t)28;
        result = val / (seq_t)18;
    } else { /* rmod == 2 */
        result = ((seq_t)n + (seq_t)1) / (seq_t)3;
    }
    return result;
}
