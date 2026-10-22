/*
 * A326478  a(n) =
 *          n*denominator(n*Bernoulli(n-1))/denominator(Bernoulli(n-1)).
 * https://oeis.org/A326478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / gcd(n, D(n-1)) where D(k) is the denominator of the *
 *            Bernoulli number B_k. For k=0 D=1, for k=1 D=2, for odd k>1 D=1,
 *            * and for even k the primes p with p-1|k give D = ∏ p.
 *            Simplifying, * a(n)=n for even n>2, a(2)=1, and for odd n>1 *
 *            a(n)= n divided by every distinct prime factor p o...
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* provided gcd for seq_t, but not needed here */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1)=1 */
    if (n == 2) return (seq_t)1;          /* a(2)=1, special case, B1 denom=2 */

    if ((n & 1) == 0) /* even n > 2 */   /* denominator B_{n-1}=1, a(n)=n */
        return (seq_t)n;

    /* odd n >= 3 */
    idx_t k = n - 1;                      /* k is even */
    seq_t res = (seq_t)n;
    idx_t tmp = n;

    /* handle factor 2 */
    if (tmp % 2 == 0) {
        if (k % 1 == 0) /* always true for k even */
            res /= 2;
        while (tmp % 2 == 0) tmp /= 2;
    }

    /* handle odd prime factors */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            if (k % (p - 1) == 0)
                res /= p;
            while (tmp % p == 0) tmp /= p;
        }
    }

    if (tmp > 1) { /* remaining prime factor */
        if (k % (tmp - 1) == 0)
            res /= tmp;
    }

    return res;
}
