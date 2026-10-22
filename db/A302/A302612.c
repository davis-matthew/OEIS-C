/*
 * A302612  a(n) = (n+1)*(n^4-4*n^3+11*n^2-8*n+12)/12.
 * https://oeis.org/A302612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 94, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A302612
 * Formula: a(n) = (n+1)*(n^4-4*n^3+11*n^2-8*n+12)/12
 * Offset: 0
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                 /* convert index to seq_t */
    seq_t n2 = sn * sn;                  /* n^2 */
    seq_t n3 = n2 * sn;                  /* n^3 */
    seq_t n4 = n3 * sn;                  /* n^4 */
    seq_t expr = n4 - (seq_t)4 * n3 +      /* n^4 - 4*n^3 + ... */
                 (seq_t)11 * n2 - (seq_t)8 * sn + (seq_t)12;
    seq_t result = (sn + 1) * expr / (seq_t)12;
    return result;
}
