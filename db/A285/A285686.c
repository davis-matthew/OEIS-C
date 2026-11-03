/*
 * A285686  Characteristic sequence of the Beatty sequence, A022840, of
 *          sqrt(6).
 * https://oeis.org/A285686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)/sqrt(6)) - floor(n/sqrt(6))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9790 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using integer arithmetic only, no floating point. */

#include <stdint.h>
#include <limits.h>

/* Computes floor(sqrt(floor(n*n/6))) for idx_t n using 128‑bit arithmetic. */
static inline seq_t floor_sqrt_div6_u64(idx_t n)
{
    /* 128‑bit intermediate to avoid overflow of n*n. */
    __uint128_t sq   = (__uint128_t)n * n;
    __uint128_t div6 = sq / 6;

    /* Binary search for integer sqrt in 64‑bit range. */
    unsigned long long lo = 0, hi = ULLONG_MAX;
    while (lo <= hi) {
        unsigned long long mid = (lo + hi) >> 1;
        __uint128_t msq = (__uint128_t)mid * mid;
        if (msq <= div6) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return (seq_t)hi;
}

/* Characteristic sequence A285686. */
seq_t a(idx_t n)
{
    seq_t k1 = floor_sqrt_div6_u64(n);
    seq_t k2 = floor_sqrt_div6_u64((idx_t)(n + 1));
    return k2 - k1;
}
