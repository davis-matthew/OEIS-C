/*
 * A368707  Number of length-n overlap-free binary words that are squares.
 * https://oeis.org/A368707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 if n == 2 a(n) = 2^(v2(n)-1) if n is a power of two
 *            with v2(n)≥2 a(n) = n if n = 3·2^k with k≥1 a(n) = 0 otherwise
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    if (n == 2) return 2;

    /* v2(n) = exponent of 2 in n */
    idx_t tmp = n;
    unsigned v2 = 0;
    while ((tmp & 1) == 0) {
        v2++;
        tmp >>= 1;
    }

    /* n is a power of two */
    if (tmp == 1 && v2 >= 2) {
        return ((seq_t)1) << (v2 - 1);
    }

    /* n = 3·2^k case */
    if (n % 3 == 0) {
        idx_t m = n / 3;
        tmp = m;
        unsigned v2m = 0;
        while ((tmp & 1) == 0) {
            v2m++;
            tmp >>= 1;
        }
        if (tmp == 1 && v2m >= 1) {
            return (seq_t)n;
        }
    }

    return 0;
}
