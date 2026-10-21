/*
 * A278086  1/12 of the number of integer quadruples with sum = 3*n and sum of
 *          squares = 7*n^2.
 * https://oeis.org/A278086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative function with a(2) = 1, a(2^k)=0 (k>=2)
 *            a(p^k) = p^(k-1)*a(p) for odd prime p, a(p) = p+1 if p mod 19 in
 *            {2,3,8,10,12,13,14,15,18}, a(p) = p-1 otherwise, a(19)=19
 * Range:     u8 n <= 200, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* index offset starts at 1; value for 0 unused */
    seq_t result = 1;
    idx_t m = n;

    /* Handle factor 2 */
    if ((m & 1ULL) == 0) {
        unsigned k2 = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++k2;
        }
        if (k2 >= 2)                /* a(2^k)=0 for k>=2 */
            return 0;               /* whole product becomes zero */
        /* k2==1: a(2)=1, no change to result */
    }

    /* Factor odd primes */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned kp = 0;
            while (m % p == 0) {
                m /= p;
                ++kp;
            }

            idx_t   r   = p % 19;
            seq_t   ap;
            if (r == 0) {
                ap = (seq_t)19;
            } else if (r == 2 || r == 3 || r == 8 || r == 10 ||
                       r == 12 || r == 13 || r == 14 || r == 15 || r == 18) {
                ap = (seq_t)p + 1;
            } else {
                ap = (seq_t)p - 1;
            }

            seq_t pow = 1;                    /* p^(kp-1) */
            for (unsigned i = 1; i < kp; ++i)
                pow *= (seq_t)p;

            result *= pow * ap;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        idx_t   p   = m;
        idx_t   r   = p % 19;
        seq_t   ap;

        if (r == 0) {
            ap = (seq_t)19;
        } else if (r == 2 || r == 3 || r == 8 || r == 10 ||
                   r == 12 || r == 13 || r == 14 || r == 15 || r == 18) {
            ap = (seq_t)p + 1;
        } else {
            ap = (seq_t)p - 1;
        }

        /* exponent kp = 1 => p^(kp-1) = 1 */
        result *= ap;
    }

    return result;
}
