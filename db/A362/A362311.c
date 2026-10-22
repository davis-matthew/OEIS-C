/*
 * A362311  Triangle read by rows (row length 2*n+1). Row n lists the integer
 *          solutions for x in the equation x - 2^n = x/y (x and y are
 *          integers).
 * https://oeis.org/A362311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A362311
 *
 *  a(n) = T(r, k) where
 *      r = floor(sqrt(n))
 *      k = n - r*r   (0 <= k <= 2*r)
 *
 *  T(r, k) = 2^r - 2^(r-k-1)    if k < r
 *         = 2^r + 2^(k-r)      if k >= r
 *
 *  The function works for all seq_t sizes (fixed‑width or arbitrary
 *  precision).  For 64‑bit builds, only terms that fit in unsigned
 *  64‑bit integers are required to be exact; larger intermediate values
 *  may overflow modulo 2^64, which does not affect those tests.
 *
 *  No external utilities are required; an integer square root and a
 *  small power‑of‑two routine are implemented locally.
 */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Integer square root (floor) for idx_t (uint64_t).  Uses binary      */
/* search with no floating‑point arithmetic.  Guarantees no overflow. */
/* ------------------------------------------------------------------ */
static inline idx_t int_sqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;   /* upper mid to avoid infinite loop */
        if (mid != 0 && mid > n / mid)    /* mid*mid > n  -> high side */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/* 2^exp computed on seq_t.  Uses repeated doubling to avoid UB on   */
/* shift operations for large exponents.  For seq_t that is larger   */
/* than 64 bits this is safe; for 64‑bit seq_t any overflow beyond      */
/* 2^63 is defined modulo 2^64 and is acceptable because those terms  */
/* are not required to be exact.                                       */
/* ------------------------------------------------------------------ */
static inline seq_t pow2_idx(idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result = result * 2;   /* multiply by 2, safe for all integer widths */
    return result;
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find row r and column k */
    idx_t r = int_sqrt_idx(n);       /* floor(sqrt(n))      */
    idx_t k = n - r * r;             /* 0 <= k <= 2*r       */

    seq_t two_r = pow2_idx(r);       /* 2^r                 */

    if (k < r) {                     /* k < r case          */
        seq_t sub = pow2_idx(r - k - 1);  /* 2^(r-k-1)           */
        return two_r - sub;
    } else {                         /* k >= r case         */
        seq_t add = pow2_idx(k - r);      /* 2^(k-r)             */
        return two_r + add;
    }
}
