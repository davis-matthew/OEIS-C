/*
 * A341844  Least k > 1 such that (n+k^n)/(n+k) is an integer.
 * https://oeis.org/A341844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 1 such that (n + kⁿ)/(n + k) is integer
 * Range:     u8 n <= 269, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;           /* convert index to 64‑bit integer */
    for (uint64_t k = 2; ; ++k) {       /* search k > 1 */
        uint64_t m = N + k;             /* m = n + k */
        uint64_t mod = m;
        uint64_t result = 1;            /* result = kⁿ mod m (will be computed) */
        uint64_t base = k;              /* base = k (since k < m) */
        idx_t exp = n;
        while (exp) {                   /* fast modular exponentiation */
            if (exp & 1)
                result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        if ((result + N) % mod == 0)    /* (n + kⁿ) divisible by n + k */
            return (seq_t)k;            /* cast to seq_t (type may be 8‑, 16‑, etc.) */
    }
}
