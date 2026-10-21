/*
 * A294275  Sum of the eighth powers of the parts in the partitions of n into
 *          two parts.
 * https://oeis.org/A294275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (i^8 + (n-i)^8)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 176, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* external types:
   typedef uint64_t idx_t;
   typedef unsigned __int128 seq_t;  // or other integer type per build
*/
seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t half = n >> 1;                 /* floor(n/2) */
    for (idx_t i = 1; i <= half; ++i) {
        seq_t ii = (seq_t)i;
        /* ii^8 = ii^2 ^ 4 -> (ii^2)*(ii^2)*(ii^2)*(ii^2) */
        seq_t ii2 = ii * ii;             /* i^2 */
        seq_t ii4 = ii2 * ii2;           /* i^4 */
        seq_t ii8 = ii4 * ii4;           /* i^8 */

        idx_t j = n - i;
        seq_t jj = (seq_t)j;
        seq_t jj2 = jj * jj;
        seq_t jj4 = jj2 * jj2;
        seq_t jj8 = jj4 * jj4;

        result += ii8 + jj8;
    }
    return result;
}
