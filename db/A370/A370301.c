/*
 * A370301  Least number of vertices of a universal graph for cycles up to
 *          length n, i.e., a graph containing induced cycles of lengths 3..n.
 * https://oeis.org/A370301
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   Let L = floor(log2(n-1)). If n is a power of two or n = 3·2^k +
 *            1 for some k ≥ 0 a(n) = n + L otherwise a(n) = n + L – 1 (Valid
 *            for all n ≥ 3.)
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute L = floor(log2(n-1)) */
    idx_t t   = n - 1;
    idx_t L   = 0;
    idx_t tmp = t;
    while (tmp > 1) {
        tmp >>= 1;
        ++L;
    }

    /* test if n is a power of two */
    bool special = ((n & (n - 1)) == 0);

    /* test if n = 3·2^k + 1  <=>  (n-1) divisible by 3 and (n-1)/3 is a power of two */
    if (!special && (t % 3) == 0) {
        idx_t y = t / 3;
        if ((y & (y - 1)) == 0) {
            special = true;
        }
    }

    seq_t result = (seq_t)n + (seq_t)L;
    if (!special)
        result -= (seq_t)1;

    return result;
}
