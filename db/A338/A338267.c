/*
 * A338267  a(n) is the nearest integer to the area of a triangle with sides
 *          prime(n), prime(n+1), prime(n+2).
 * https://oeis.org/A338267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round( sqrt( (prime(n)+prime(n+1)+prime(n+2))/2 * *
 *            (s-prime(n))*(s-prime(n+1))*(s-prime(n+2)) ) )
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 5187, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where s = (prime(n)+prime(n+1)+prime(n+2))/2,
 * implemented as a(n) = round( sqrt( (prime(n)+prime(n+1)+prime(n+2)) *
 * (prime(n)+prime(n+1)+prime(n+2)-2*prime(n)) *
 * (prime(n)+prime(n+1)+prime(n+2)-2*prime(n+1)) *
 * (prime(n)+prime(n+1)+prime(n+2)-2*prime(n+2)) ) / 4 ) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */

static inline uint64_t isqrt128(unsigned __int128 x)
{
    /* binary search for floor sqrt of 128‑bit integer (result < 2^64) */
    uint64_t lo = 0, hi = (uint64_t)(-1);
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == x) return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* primes at positions n, n+1, n+2 (1‑based) */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);
    uint32_t p3 = get_prime(n + 2);

    /* compute M = (p1+p2+p3)*(p1+p2+p3-2*p1)*(p1+p2+p3-2*p2)*(p1+p2+p3-2*p3) */
    seq_t t  = (seq_t)p1 + (seq_t)p2 + (seq_t)p3;
    seq_t t1 = t;
    seq_t t2 = t - (seq_t)2 * p1;
    seq_t t3 = t - (seq_t)2 * p2;
    seq_t t4 = t - (seq_t)2 * p3;

    unsigned __int128 M128 = (unsigned __int128)t1
                           * (unsigned __int128)t2
                           * (unsigned __int128)t3
                           * (unsigned __int128)t4;

    /* sqrt(M) fits in 64 bits (for all n of interest) */
    uint64_t sqrtM = isqrt128(M128);
    seq_t s = (seq_t)sqrtM;         /* floor(sqrt(M)) */

    /* a(n) = round(sqrt(M)/4)  ->  floor((s + 2)/4) */
    seq_t result = s / 4;
    if ((s % 4) >= 2)
        result += 1;
    return result;
}
