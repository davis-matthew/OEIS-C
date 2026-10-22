/*
 * A380237  Number of sensed planar maps with n vertices and 2 faces.
 * https://oeis.org/A380237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( 1/(2*n) ) * ( n * binomial(n-1, floor((n-1)/2)) * +
 *            Sum_{d|n} φ(n/d) * ( 2^(2*d-1) - binomial(2*d-1, d) ) )
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   binomial, phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a small 128‑bit helper type for the first
 * 35 values (which cover all 64‑bit results).  For larger n it
 * falls back to the standard seq_t arithmetic which works for the
 * arbitrary‑precision build. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/phi.h"
#include "../utils/pow2.h"

/* 128‑bit helper (two 64‑bit limbs) */
typedef struct { uint64_t lo; uint64_t hi; } uv128;

static inline uv128 uv128_from_uint64(uint64_t x) {
    uv128 r = { x, 0 };
    return r;
}
static inline uv128 uv128_add(uv128 a, uv128 b) {
    uv128 r;
    r.lo = a.lo + b.lo;
    uint64_t carry = r.lo < a.lo;
    r.hi = a.hi + b.hi + carry;
    return r;
}
static inline uv128 uv128_sub(uv128 a, uv128 b) {  /* assumes a>=b */
    uv128 r;
    uint64_t borrow = a.lo < b.lo;
    r.lo = a.lo - b.lo;
    r.hi = a.hi - b.hi - borrow;
    return r;
}
static inline uv128 uv128_mul64(uv128 a, uint64_t b) {
    unsigned __int128 lo128 = (unsigned __int128)a.lo * b;
    uv128 r;
    r.lo = (uint64_t)lo128;
    unsigned __int128 hi128 = (unsigned __int128)a.hi * b + (lo128 >> 64);
    r.hi = (uint64_t)hi128;
    return r;
}

/* binomial for n <= 70 using 128‑bit arithmetic */
static uv128 binom_uv128(uint64_t n, uint64_t k) {
    if (k > n) return uv128_from_uint64(0);
    if (k > n - k) k = n - k;
    uv128 res = uv128_from_uint64(1);
    for (uint64_t i = 1; i <= k; ++i) {
        unsigned __int128 tmp = ((unsigned __int128)res.hi << 64) | res.lo;
        tmp *= (n - k + i);
        tmp /= i;                /* division is exact */
        res.lo = (uint64_t)tmp;
        res.hi = (uint64_t)(tmp >> 64);
    }
    return res;
}

/* 2^(exp) for exp < 128, expressed as uv128 */
static uv128 pow2_uv128(uint64_t exp) {
    uv128 r = {0,0};
    if (exp < 64) {
        r.lo = ((uint64_t)1 << exp);
    } else {
        r.hi = ((uint64_t)1 << (exp - 64));
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* out of range, not used */

    /* For n <= 35 use the 128‑bit helper to avoid 64‑bit overflow. */
    if (n <= 35) {
        uint64_t N = (uint64_t)n;
        uint64_t k = (N - 1) >> 1;     /* floor((n-1)/2) */
        uv128 binom1 = binom_uv128(N - 1, k);
        uv128 term1 = uv128_mul64(binom1, N);

        uv128 sum = uv128_from_uint64(0);

        for (idx_t i = 1; i * i <= n; ++i) {
            if (n % i == 0) {
                /* divisor d = i */
                uint64_t d = (uint64_t)i;
                uint64_t phi_val = (uint64_t)phi((idx_t)(n / d));

                uv128 pow_part = pow2_uv128(2 * d - 1);
                uv128 binom_part = binom_uv128(2 * d - 1, d);
                uv128 diff = uv128_sub(pow_part, binom_part);
                uv128 term = uv128_mul64(diff, phi_val);
                sum = uv128_add(sum, term);

                /* second divisor d = n/i if distinct */
                if (i != n / i) {
                    uint64_t d2 = (uint64_t)(n / i);
                    uint64_t phi_val2 = (uint64_t)phi((idx_t)(n / d2));

                    uv128 pow_part2 = pow2_uv128(2 * d2 - 1);
                    uv128 binom_part2 = binom_uv128(2 * d2 - 1, d2);
                    uv128 diff2 = uv128_sub(pow_part2, binom_part2);
                    uv128 term2 = uv128_mul64(diff2, phi_val2);
                    sum = uv128_add(sum, term2);
                }
            }
        }

        uv128 numer = uv128_add(term1, sum);
        uint64_t denom = 2 * N;               /* denominator fits in 64‑bit */
        unsigned __int128 num128 = ((unsigned __int128)numer.hi << 64) | numer.lo;
        unsigned __int128 q128 = num128 / denom;
        return (seq_t)q128;
    }

    /* For larger n – use full seq_t arithmetic.  Overflows may occur
     * for the 64‑bit build, but such n are not tested for that build. */
    idx_t k = (n - 1) / 2;
    seq_t term1 = binomial(n - 1, k);
    term1 *= (seq_t)n;

    seq_t sum = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d = i */
            idx_t d = i;
            seq_t phi_val = phi(n / d);
            seq_t pow_part = pow2(2 * d - 1);
            seq_t binom_part = binomial(2 * d - 1, d);
            seq_t diff = pow_part - binom_part;
            seq_t term = phi_val * diff;
            sum += term;

            /* second divisor d = n/i if distinct */
            if (i != n / i) {
                idx_t d2 = n / i;
                seq_t phi_val2 = phi(n / d2);
                seq_t pow_part2 = pow2(2 * d2 - 1);
                seq_t binom_part2 = binomial(2 * d2 - 1, d2);
                seq_t diff2 = pow_part2 - binom_part2;
                seq_t term2 = phi_val2 * diff2;
                sum += term2;
            }
        }
    }

    seq_t numer = term1 + sum;
    seq_t denom = (seq_t)(2 * n);
    return numer / denom;
}
