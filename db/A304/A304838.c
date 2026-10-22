/*
 * A304838  a(n) = 1944*n^2 - 5016*n + 3138 (n>=1).
 * https://oeis.org/A304838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1944*n^2 - 5016*n + 3138 (n>=1)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) {
        /* undefined for n < 1; return 0 as a safe default */
        return 0;
    }
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)1944 * nn * nn;
    seq_t term2 = (seq_t)5016 * nn;
    seq_t result = term1 - term2 + (seq_t)3138;
    return result;
}
