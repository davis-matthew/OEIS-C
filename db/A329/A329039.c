/*
 * A329039  If n = Product p_i^e_i, a(n) = n * Sum ((e_i - 1)/p_i).
 * https://oeis.org/A329039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * sum((e_i - 1)/p_i)
 * Range:     u8 n <= 127, u16 n <= 8812, u32 n <= 8812, u64 n <= 8812,
 *            u128 n <= 8812, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t m = (uint64_t)n;   /* work on a 64‑bit copy of n */
    seq_t   nn = (seq_t)n;      /* cast once for later arithmetic */
    seq_t   res = (seq_t)0;

    /* factor out the power of 2 */
    int e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    if (e > 1)
        res += (nn / (seq_t)2) * ((seq_t)(e - 1));

    /* factor odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e > 1)
                res += (nn / (seq_t)p) * ((seq_t)(e - 1));
        }
    }

    /* if m > 1 it is a prime factor with exponent 1 – no contribution */
    return res;
}
