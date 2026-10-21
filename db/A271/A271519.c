/*
 * A271519  Let n = (2*i + 1)*2^j; then a(n) = i + j.
 * https://oeis.org/A271519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ((odd(n)-1)/2) + v2(n), where odd(n) is n with all
 *            factors of 2 removed (i.e., n = odd(n) * 2^v2(n))
 * Range:     u8 n <= 512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety, though n>=1 by definition */
    idx_t j = 0;                   /* exponent of 2 (v2(n)) */
    idx_t m = n;
    while ((m & 1) == 0) {         /* strip powers of 2 */
        m >>= 1;
        j++;
    }
    seq_t odd = (seq_t)m;          /* odd part of n */
    seq_t i = (odd - 1) / 2;       /* compute i = (odd-1)/2 */
    return i + (seq_t)j;           /* a(n) = i + j */
}
