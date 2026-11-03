/*
 * A387658  Number of points on a cubic lattice that have a minimum distance
 *          sqrt(n) from either of two adjacent lattice points.
 * https://oeis.org/A387658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Sum_{i=0..⌊√n⌋} r2(n-i²), where r2(m) = number of *
 *            representations of m as a sum of two squares (ordered, signed).
 *            * r2(0)=1; for m>0, r2(m)=0 if any prime ≡3 (mod4) occurs to an
 *            odd * exponent, otherwise r2(m)=4∏(exp+1) over primes ≡1 (mod4).
 *            * * Implementation uses trial division for...
 * Range:     u8 n <= 268, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9772 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* r2(m): number of integer representations of m as x²+y² (ordered, signed). */
static seq_t r2_value(idx_t m) {
    if (m == 0) return (seq_t)1;          /* only (0,0) */
    idx_t temp = m;
    seq_t prod = (seq_t)1;
    /* factorisation */
    while (temp > 1) {
        idx_t p;
        if (temp & 1) {                      /* temp odd */
            /* try to find smallest odd divisor */
            p = 3;
            while (p * p <= temp) {
                if (temp % p == 0) break;
                p += 2;
            }
            if (p * p > temp) p = temp;      /* temp itself is prime */
        } else {
            p = 2;
        }
        /* count exponent of p */
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }
        if (p % 4 == 3) {
            if (exp & 1) return (seq_t)0;    /* odd exponent → impossible */
        } else if (p % 4 == 1) {
            prod = prod * (seq_t)(exp + 1);
        } /* p == 2 contributes nothing */
    }
    return (seq_t)4 * prod;
}

static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    idx_t lim = isqrt_u64(n);
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i <= lim; ++i) {
        idx_t m = n - i * i;
        sum += r2_value(m);
    }
    return sum + sum;                       /* 2 * sum */
}
