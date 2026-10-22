/*
 * A359548  Dirichlet inverse of A053866, where A053866(n) gives the parity of
 *          sigma(n).
 * https://oeis.org/A359548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 9847, i16 n <= 9847, i32 n <= 9847, i64 n <= 9847,
 *            i128 n <= 9847, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A359548: Dirichlet inverse of A053866
    Multiplicative definition:
        a(1) = 1
        a(2)  = -1   (2^1)
        a(2^e) = 0   for e > 1
        a(p^2) = -1  for odd primes p
        a(p^e) = 0   for e = 1 or e > 2
    Consequently a(n) = 0 unless n = 2^e * ∏_p p^2 (p odd, e ∈ {0,1});
    In that case a(n) = (-1)^(number of odd primes + e).  */

#include <stdint.h>
#include <stddef.h>

/* integer square root for unsigned 64-bit values, using binary search */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;          /* base case */

    uint64_t m = n;        /* working copy, since a() may be called in any order */
    seq_t sign = 1;        /* will be ±1 */

    /* trial division up to sqrt(m); m shrinks over time */
    idx_t limit = isqrt_u64(m);
    for (uint64_t p = 2; p <= (uint64_t)limit; ++p) {
        if (m % p == 0) {
            size_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (p == 2) {
                if (exp == 1)
                    sign = -sign;
                else
                    return 0;            /* 2^e with e>1 forces zero */
            } else {
                if (exp == 2)
                    sign = -sign;
                else
                    return 0;            /* odd prime exponent not equal 2 forces zero */
            }
            limit = isqrt_u64(m);        /* update the search bound after reduction */
        }
    }

    /* any remaining factor > 1 is prime */
    if (m > 1) {
        if (m == 2) {
            sign = -sign;                 /* remaining exponent is 1 for prime 2 */
        } else {
            /* odd prime exponent is 1, which causes zero */
            return 0;
        }
    }

    return sign;
}
