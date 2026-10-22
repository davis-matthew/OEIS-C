/*
 * A348972  a(n) = gcd(A003959(n), A129283(n)), where A003959 is
 *          multiplicative with a(p^e) = (p+1)^e and A129283(n) is sum of n
 *          and its arithmetic derivative.
 * https://oeis.org/A348972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003959(n), A129283(n)) where A003959(n)=∏(p+1)^e for
 *            n=∏p^e and A129283(n)=n+d(n), d(n)=n·∑e/p
 * Range:     u8 n <= 63, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* (offset is 1, but guard) */

    uint64_t nn   = (uint64_t)n;   /* original value, for use in derivative */
    uint64_t tmp  = nn;            /* factorization work */
    seq_t  a003959_val = 1;        /* multiplicative value */
    seq_t  d_val       = 0;        /* arithmetic derivative */

    /* handle factor 2 */
    if (tmp % 2 == 0) {
        int e = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            ++e;
        }
        for (int i = 0; i < e; ++i)
            a003959_val *= (seq_t)3;   /* (2+1) = 3 */
        d_val += ((seq_t)nn / 2) * (seq_t)e;
    }

    /* odd primes */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t pp1 = (seq_t)(p + 1);
            for (int i = 0; i < e; ++i)
                a003959_val *= pp1;
            d_val += ((seq_t)nn / p) * (seq_t)e;
        }
    }

    /* remaining prime factor > 1 */
    if (tmp > 1) {
        int e = 1;
        seq_t pp1 = (seq_t)(tmp + 1);
        a003959_val *= pp1;
        d_val += ((seq_t)nn / tmp);
    }

    seq_t sum_val = (seq_t)n + d_val;   /* A129283(n) */
    return gcd(a003959_val, sum_val);
}
