/*
 * A374880  Obverse convolution (floor(3n/2))**(floor(3n/2)); see Comments.
 * https://oeis.org/A374880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k) = (3k)^(k+1) * (3k-1)^k, a(2k+1) = (3k+1)^(2k+2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed with exponentiation by squaring, avoiding helper routines. */
#include <stdint.h>   /* for the provided seq_t and idx_t types */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* m = floor(3n/2) */
    seq_t m = ((seq_t)n * 3) / 2;

    if (n & 1) {                 /* odd n = 2k+1 */
        idx_t exp = n + 1;        /* exponent = 2k+2 */
        return pow_seq(m, exp);
    } else {                     /* even n = 2k */
        idx_t k = n / 2;
        seq_t term1 = pow_seq(m, k + 1);          /* m^(k+1) */
        seq_t term2 = pow_seq(m - 1, k);          /* (m-1)^k */
        return term1 * term2;
    }
}
