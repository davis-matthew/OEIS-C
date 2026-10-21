/*
 * A286090  Square matrix m read by antidiagonals. For j <= i, m[i, j] is how
 *          often sqrtint(gpf(c)) = j where i^2 <= c < (i+1)^2; for j >= i,
 *          m[i, j] is how often sqrtint(p) = i where p is a prime factor of c
 *          counted with multiplicity and i^2 <= c < (i+1)^2.
 * https://oeis.org/A286090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n-th entry of the square matrix read by
 *            antidiagonals. The matrix element m[i,j] is defined as follows.
 *            Let k = max(i,j) and consider the interval c ∈ [k² , (k+1)² –
 *            1]. - If j ≤ i: m[i,j] is the number of integers c in that
 *            interval for which floor(√(gpf(c))) = j. - If j ≥ i: m[i,j] i...
 * Range:     u8 n <= 3486, u16 n <= 9800, u32 n <= 9800, u64 n <= 9800,
 *            u128 n <= 9800, big any n
 * Helpers:   ceil_tri_inverse, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gpf.h"
#include "../utils/ceil_tri_inverse.h"

/* Integer square root of a 64‑bit unsigned integer. */
static inline idx_t isqrt_u64(uint64_t n)
{
    idx_t lo = 0, hi = (idx_t)1ULL << 32;          /* hi is exclusive */
    while (lo + 1 < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((__int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Count prime factors p of x (with multiplicity) such that floor(√p) == target. */
static inline idx_t count_primes_with_sqrt(uint64_t x, idx_t target)
{
    idx_t cnt = 0;

    /* factor 2 */
    while ((x & 1ULL) == 0) {
        if (target == 1)   /* sqrt(2)=1 */
            cnt++;
        x >>= 1;
    }

    /* odd factors */
    for (uint64_t p = 3; p <= x / p; p += 2) {
        while (x % p == 0) {
            if (isqrt_u64(p) == target)
                cnt++;
            x /= p;
        }
    }

    if (x > 1) {
        if (isqrt_u64(x) == target)
            cnt++;
    }

    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                            /* offset 1 – n<1 not used */

    /* Compute the matrix coordinates (i,j) for the given index n. */
    idx_t s   = ceil_tri_inverse(n);           /* s = i + j - 1  (from n = T_{s-1} + i) */
    idx_t t   = (s - 1) * s / 2;               /* T_{s-1} */
    idx_t i   = n - t;                         /* row index */
    idx_t j   = s + 1 - i;                     /* column index */
    idx_t k   = (i > j) ? i : j;               /* maximal index for c interval */

    /* Interval of c values: [k² , (k+1)² – 1] */
    uint64_t L = (uint64_t)k * k;
    uint64_t R = (uint64_t)(k + 1) * (k + 1) - 1;

    seq_t total = 0;

    if (j <= i) {                              /* use greatest prime factor */
        for (uint64_t c = L; c <= R; ++c) {
            uint64_t h = gpf(c);
            if (isqrt_u64(h) == j)
                total++;
        }
    } else {                                   /* use prime‑factor counting */
        for (uint64_t c = L; c <= R; ++c) {
            /* Count prime factors of c whose sqrt equals i. */
            total += (seq_t)count_primes_with_sqrt(c, i);
        }
    }

    return total;
}
