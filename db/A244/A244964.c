/*
 * A244964  Number of distinct generalized pentagonal numbers dividing n.
 * https://oeis.org/A244964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct generalized pentagonal numbers
 *            dividing n
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the number of distinct generalized pentagonal numbers that divide n. */
seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t p1 = (seq_t)k * ((seq_t)3 * k - 1) / 2;
        if (p1 > n) break;
        if (n % (idx_t)p1 == 0) cnt++;
        seq_t p2 = (seq_t)k * ((seq_t)3 * k + 1) / 2;
        if (p2 <= n && n % (idx_t)p2 == 0) cnt++;
    }
    return cnt;
}
