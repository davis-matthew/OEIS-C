/*
 * A285119  Min(|d(k+1-i) - d(i)|, for i = 1..k), where d(1)..d(k) are the
 *          divisors of n^3.
 * https://oeis.org/A285119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = Min(|d(k+1-i) - d(i)|, for i = 1..k), where d(1)..d(k)
 *            are the divisors of n³ (sorted ascending)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------
   Helper: integer square‑root (floor) for 64‑bit values.
   Uses binary search, no division by zero is performed.
   ------------------------------------------------------------------ */
static uint64_t isqrt_u64(uint64_t v)
{
    uint64_t lo = 0, hi = v, mid;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        uint64_t sq = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* ------------------------------------------------------------------
   Division & modulus of 64‑bit are safe for n <= 10000,
   but we guard against 0‑division for completeness.
   ------------------------------------------------------------------ */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    return (*x > *y) - (*x < *y);
}

/* ------------------------------------------------------------------
   Main function: a(n)
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* 1. Handle perfect squares (including 1) */
    if (n > 0) {
        uint64_t r = isqrt_u64(n);
        if (r * r == n) return (seq_t)0;
    }

    /* 2. Factorise n (trial division, n <= 10000) */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[20];
    int exps[20];
    size_t pcnt = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            primes[pcnt] = p;
            exps[pcnt]   = cnt;
            ++pcnt;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* 3. Generate all divisors of n³ */
    size_t div_cnt = 1;
    uint64_t *divs = (uint64_t *)malloc(div_cnt * sizeof(uint64_t));
    divs[0] = 1;

    for (size_t idx = 0; idx < pcnt; ++idx) {
        uint64_t p       = primes[idx];
        int      exp     = exps[idx] * 3;             /* exponent in n³ */
        size_t   old_cnt = div_cnt;
        size_t   new_cnt = old_cnt * (exp + 1);
        uint64_t *new_divs = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));

        for (size_t j = 0; j < old_cnt; ++j) {
            __uint128_t prod = 1;
            for (int t = 0; t <= exp; ++t) {
                new_divs[j * (exp + 1) + t] = (uint64_t)(divs[j] * prod);
                prod *= p;
            }
        }

        free(divs);
        divs     = new_divs;
        div_cnt  = new_cnt;
    }

    /* 4. Sort the divisor list */
    qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* 5. Find minimal absolute difference of mirrored pairs */
    uint64_t best = UINT64_MAX;
    for (size_t i = 0; i < div_cnt / 2; ++i) {
        uint64_t left  = divs[i];
        uint64_t right = divs[div_cnt - 1 - i];
        uint64_t diff  = right - left;                 /* left <= right */
        if (diff < best) best = diff;
    }

    free(divs);
    return (seq_t)best;
}
