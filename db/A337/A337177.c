/*
 * A337177  Sum of the divisors d of n such that d is not equal to n/d.
 * https://oeis.org/A337177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) minus sqrt(n) if n is a perfect square, where
 *            sigma(n) is the sum of all positive divisors of n
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(n)) for 0 <= n <= UINT64_MAX */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low  = mid + 1;
        else
            high = mid - 1;
    }
    return high;                /* floor(sqrt(n)) */
}

/* sum of divisors sigma(n) for a 64‑bit unsigned integer n */
static seq_t sigma_u64(uint64_t n)
{
    seq_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t sigma_p = 1;  /* sigma of current prime power */
            seq_t power = 1;
            while (m % p == 0) {
                m /= p;
                power *= (seq_t)p;   /* p^k */
                sigma_p += power;    /* 1 + p + p^2 + ... */
            }
            result *= sigma_p;
        }
    }
    if (m > 1) {                 /* remaining prime factor */
        result *= ((seq_t)1 + (seq_t)m);
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t tn = (uint64_t)n;         /* n >= 1 */
    if (tn == 0)                       /* safety for out‑of‑range calls */
        return 0;

    seq_t sigma = sigma_u64(tn);
    uint64_t sq = isqrt_u64(tn);       /* floor(sqrt(tn)) */

    if ((__int128)sq * sq == tn)       /* perfect square: subtract the middle divisor */
        sigma -= (seq_t)sq;

    return sigma;
}
