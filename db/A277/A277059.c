/*
 * A277059  Least k such that any sufficiently long repunit multiplied by k
 *          contains all nonzero digits in base n.
 * https://oeis.org/A277059
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = (n^m - 1)/(n - 1) + m - 1 for n = 2m a(n) =
 *            ((n^(2m)-1)(n^2+1))/(2(n^2-1)) + m for n = 4m+1 a(n) =
 *            ((n^(2m-2)-1)(n^2+1))/(2(n^2-1)) + m + n^(2m-1) for n ≥ 2. The
 *            conjectured formulas give the first terms of A277059
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n_val)
{
    /* n is small (≤ 10 000 in benchmark), so we can use 64‑bit
       arithmetic to compute the exponents.  seq_t may be any
       unsigned integer type (including arbitrary precision). */
    if (n_val < 2) return 0;          /* out of offset */
    uint64_t n_u = (uint64_t)n_val;   /* base always fits in 64 bit */
    seq_t n = (seq_t)n_u;

    seq_t result;
    /* Even n = 2m */
    if ((n_u & 1ULL) == 0) {
        idx_t m = n_u >> 1;         /* m = n/2 */
        seq_t pow_nm = pow_uint(n_u, m);              /* n^m */
        seq_t term = (pow_nm - 1) / (n - 1);         /* (n^m-1)/(n-1) */
        result = term + (seq_t)(m - 1);
    }
    /* n ≡ 1 (mod 4)  → 4m+1 */
    else if ((n_u % 4ULL) == 1ULL) {
        idx_t m = (n_u - 1) / 4;    /* m = (n-1)/4 */
        idx_t two_m = 2 * m;
        seq_t pow_n2m = pow_uint(n_u, two_m);          /* n^(2m) */
        seq_t numer = pow_n2m - 1;                     /* n^(2m)-1 */
        seq_t n2 = n * n;                              /* n^2 */
        seq_t n2_plus1 = n2 + 1;                       /* n^2+1 */
        seq_t n2_minus1 = n2 - 1;                      /* n^2-1 */
        seq_t denom = 2 * n2_minus1;                   /* 2(n^2-1) */
        seq_t prod = numer * n2_plus1;                 /* (n^(2m)-1)(n^2+1) */
        seq_t div = prod / denom;                      /* divide */
        result = div + (seq_t)m;
    }
    /* n ≡ 3 (mod 4)  → 4m-1 */
    else { /* n_u % 4 == 3 */
        idx_t m = (n_u + 1) / 4;      /* m = (n+1)/4 */
        idx_t exp1 = 2 * m - 2;        /* 2m-2 (may be 0) */
        idx_t exp2 = 2 * m - 1;        /* 2m-1 */
        seq_t pow_n_exp1 = pow_uint(n_u, exp1);        /* n^(2m-2) */
        seq_t numer = pow_n_exp1 - 1;                  /* n^(2m-2)-1 */
        seq_t n2 = n * n;                              /* n^2 */
        seq_t n2_plus1 = n2 + 1;                       /* n^2+1 */
        seq_t n2_minus1 = n2 - 1;                      /* n^2-1 */
        seq_t denom = 2 * n2_minus1;                   /* 2(n^2-1) */
        seq_t prod = numer * n2_plus1;                 /* (n^(2m-2)-1)(n^2+1) */
        seq_t div = prod / denom;                      /* divide */
        seq_t pow_n_exp2 = pow_uint(n_u, exp2);        /* n^(2m-1) */
        result = div + (seq_t)m + pow_n_exp2;
    }
    return result;
}
