/*
 * A305751  Number of achiral color patterns (set partitions) in a row or
 *          cycle of length n with 5 or fewer colors (subsets).
 * https://oeis.org/A305751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15 + 20*2^(n/2) + 13*5^(n/2)) / 60 for even n (n>0) *
 *            a(n) = (3 + 2*2^((n+1)/2) + 5^((n+1)/2)) / 12 for odd n (n>0) *
 *            Special case: a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer power for arbitrary-precision seq_t */
static inline seq_t pow_uint(uint64_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t cur = (seq_t)base;
    while (exp) {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if ((n & 1) == 0) {                 /* even: n = 2m */
        idx_t m = n >> 1;
        seq_t pow2 = pow_uint(2, m);
        seq_t pow5 = pow_uint(5, m);
        seq_t num = (seq_t)15 + (seq_t)20 * pow2 + (seq_t)13 * pow5;
        return num / (seq_t)60;
    } else {                            /* odd: n = 2m-1 */
        idx_t m = (n + 1) >> 1;
        seq_t pow2 = pow_uint(2, m);
        seq_t pow5 = pow_uint(5, m);
        seq_t num = (seq_t)3 + (seq_t)2 * pow2 + pow5;
        return num / (seq_t)12;
    }
}
