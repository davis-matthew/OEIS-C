/*
 * A353800  a(n) = 1 if n is a power of prime with an even exponent, otherwise
 *          0.
 * https://oeis.org/A353800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9939, u16 n <= 9939, u32 n <= 9939, u64 n <= 9939,
 *            u128 n <= 9939, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = 1 if n is a power of a prime with an even exponent, else 0.
   In particular, a(1) = 1 since 1 = p^0 for any prime p. */

#include <stdbool.h>
#include <stdint.h>

/* Integer square root: returns floor(sqrt(n)) for nonnegative n. */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t r = 0;
    /* iterate while (r+1)^2 <= n, using safe division */
    while ((r + 1) <= n / (r + 1))
        r++;
    return r;
}

/* Return 1 if n is a prime power p^k with even exponent k (including k=0 for n=1),
   otherwise return 0. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* p^0 case */

    idx_t m = n;
    idx_t p = 0;                          /* smallest prime factor */

    /* First check divisibility by 2 */
    if ((m & 1ULL) == 0)
        p = 2;
    else {
        idx_t limit = isqrt_u64(m);
        for (idx_t d = 3; d <= limit; d += 2) {
            if (m % d == 0) {
                p = d;
                break;
            }
        }
    }

    if (p == 0) /* n is prime, exponent 1 (odd) */
        return (seq_t)0;

    unsigned int exp = 0;
    while (m % p == 0) {
        m /= p;
        exp++;
    }

    if (m != 1)                     /* more than one distinct prime factor */
        return (seq_t)0;

    /* exp is even (including exp==0 which can't happen except n==1 already handled) */
    return (seq_t)((exp & 1U) == 0 ? 1 : 0);
}
