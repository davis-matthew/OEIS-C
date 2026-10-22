/*
 * A396895  a(n) = 4*p - floor(2*sqrt(p))^2, where p = prime(n).
 * https://oeis.org/A396895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*prime(n) - floor(2*sqrt(prime(n)))^2
 * Range:     u8 n <= 588, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

/* Integer square‑root for unsigned 64‑bit values.
   Returns floor(sqrt(x)).  Works for the modest limits of this
   sequence (p ≤ 104 729).  */
static inline unsigned long long isqrt_ull(unsigned long long x)
{
    unsigned long long lo = 0, hi = x, mid, res = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        unsigned long long sq = mid * mid;
        if (sq == x) {
            return mid;
        }
        if (sq < x) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid infinite loop when x < 1 */
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* get the n‑th prime, zero‑based internal but OEIS offset 1 */
    uint32_t p32 = get_prime(n);
    unsigned long long p = (unsigned long long)p32;   /* intermediate value */

    unsigned long long fourp = p * 4ULL;              /* 4*p */
    unsigned long long t = isqrt_ull(fourp);         /* floor(2*sqrt(p)) */
    unsigned long long ans = fourp - t * t;          /* a(n) */

    return (seq_t)ans;
}
