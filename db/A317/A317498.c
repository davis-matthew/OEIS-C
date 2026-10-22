/*
 * A317498  Triangle read by rows of coefficients in expansions of (-2 +
 *          3*x)^n, where n is nonnegative integer.
 * https://oeis.org/A317498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = binom(r,k)*(-2)^(r-k)*3^k where r = ⌊(√(8*n+1)-1)/2⌋ and
 *            k = n - r*(r+1)/2
 * Range:     i8 n <= 11, i16 n <= 38, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <type_traits>
/* binomial for integral seq_t using 128‑bit intermediates */
static seq_t binom_integral(idx_t a, idx_t b) {
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        __int128 tmp = (__int128)res * (a - b + i);
        res = (seq_t)(tmp / i);
    }
    return res;
}
/* binomial for non‑integral (big‑int) seq_t */
static seq_t binom_bigint(idx_t a, idx_t b) {
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = (res * (seq_t)(a - b + i)) / (seq_t)i;
    }
    return res;
}
static inline seq_t compute_binom(idx_t a, idx_t b) {
    return std::is_integral<seq_t>::value ? binom_integral(a,b) : binom_bigint(a,b);
}
#else
/* C version – assume seq_t is an integer type */
static seq_t binom_integral(idx_t a, idx_t b) {
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        __int128 tmp = (__int128)res * (a - b + i);
        res = (seq_t)(tmp / i);
    }
    return res;
}
static inline seq_t compute_binom(idx_t a, idx_t b) {
    return binom_integral(a,b);
}
#endif

seq_t a(idx_t n) {
    /* --- find row index r --- */
    idx_t r;
    {
        long double d = sqrtl((long double)8 * (long double)n + 1.0L);
        r = (idx_t)((d - 1.0L) / 2.0L);
        /* adjust for possible floating‑point round‑off */
        while (1) {
            unsigned __int128 test = (unsigned __int128)(r + 1) * (r + 2) / 2;
            if (test <= n) {
                ++r;
            } else {
                break;
            }
        }
        while (r > 0) {
            unsigned __int128 test = (unsigned __int128)r * (r + 1) / 2;
            if (test > n) {
                --r;
            } else {
                break;
            }
        }
    }

    /* column index k */
    idx_t tot = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - tot;

    /* binomial coefficient C(r,k) */
    idx_t kk = k;
    if (kk > r - kk) kk = r - kk; /* symmetry for speed */
    seq_t binom = compute_binom(r, kk);

    /* powers (-2)^(r-k) and 3^k   (use seq_t operations only) */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < r - k; ++i) {
        pow2 = pow2 * (seq_t)(-2);
    }
    seq_t pow3 = 1;
    for (idx_t i = 0; i < k; ++i) {
        pow3 = pow3 * (seq_t)3;
    }

    /* final product */
    return binom * pow2 * pow3;
}
