/*
 * A256244  a(n) = sqrt(n + 2*A256243(n)).
 * https://oeis.org/A256244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(n)) + ( (n%2 == floor_sqrt(n)%2) ? 2 : 1 )
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; undefined for n<1 */
    seq_t sn = (seq_t)n;
    seq_t k = floor_sqrt(sn);             /* k = ⌊√n⌋ */
    idx_t parity_n = n & 1u;              /* n mod 2 */
    idx_t parity_k = (idx_t)(k & (seq_t)1u); /* k mod 2 */
    seq_t diff = (parity_n == parity_k) ? (seq_t)2 : (seq_t)1;
    return k + diff;
}
