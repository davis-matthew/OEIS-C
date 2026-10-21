/*
 * A297117  Möbius transform of A011371, n minus (number of 1's in binary
 *          expansion of n).
 * https://oeis.org/A297117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * (d - popcount(d))
 * Range:     u8 n <= 262, u16 n <= 9302, u32 n <= 9302, u64 n <= 9302,
 *            u128 n <= 9302, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where mu is the Möbius function and popcount is the number of 1’s in the binary expansion of d. */

#include <stdint.h>

/* popcount for 64-bit integers */
static unsigned popcount_u64(uint64_t x)
{
    unsigned c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
}

/* Möbius function mu(n) for n >= 1 */
static int mu_of(uint64_t n)
{
    if (n == 1) return 1;
    int cnt = 0;

    /* factor 2 */
    if ((n & 1) == 0) {
        n >>= 1;
        if ((n & 1) == 0) return 0;   /* square factor */
        cnt++;
    }

    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return 0;   /* square factor */
            cnt++;
        }
    }
    if (n > 1) cnt++;
    return (cnt & 1) ? -1 : 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1, but return 0 for safety */

    int64_t sum = 0;             /* signed intermediate to handle negative contributions */

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t q1 = n / d1;
            int mu1 = mu_of((uint64_t)q1);
            unsigned pc1 = popcount_u64((uint64_t)d1);
            sum += (int64_t)mu1 * ((int64_t)d1 - (int64_t)pc1);

            if (d1 != q1) {
                idx_t d2 = q1;
                idx_t q2 = d1;           /* n / d2 */
                int mu2 = mu_of((uint64_t)q2);
                unsigned pc2 = popcount_u64((uint64_t)d2);
                sum += (int64_t)mu2 * ((int64_t)d2 - (int64_t)pc2);
            }
        }
    }

    return (seq_t)sum;
}
