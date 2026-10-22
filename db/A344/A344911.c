/*
 * A344911  Concatenated Bessel-scaled Pascal triangles. Irregular triangle
 *          read by rows, T(n,k) with n >= 0 and 0 <= k <= (2*n*(n + 4) - 1 +
 *          (-1)^n)/8.
 * https://oeis.org/A344911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   _odd_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A344911 – Concatenated Bessel‑scaled Pascal triangles
 *
 *  The sequence is obtained by concatenating the rows
 *
 *      T(n) = Seq_{k=0..⌊n/2⌋}  B(n,k) * C(n-2k)
 *
 *  where B(n,k) = binomial(n,2k)·(2k-1)!!  (with (−1)!! = 1),
 *  and C(m) is the ordinary Pascal row (binomial coefficients).
 *  The triangle rows are read in degree‑lexicographic order.
 *
 *  The implementation below follows exactly this definition
 *  and is O(n) in the index n (≤10 000) with no external
 *  memory usage beyond the size of intermediate big integers.
 *  It works for any seq_t type (X bit unsigned or arbitrary precision)
 *  supplied by the build.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_odd_factorial.h"

/* ----- helpers ----- */

static inline idx_t _row_len(idx_t n)
{
    idx_t m = n / 2;                /* floor(n/2) */
    return (m + 1) * (n + 1 - m);   /* (⌊n/2⌋+1)*(n+1-⌊n/2⌋) */
}

/* multiplicative binomial coefficient for idx_t arguments */
static inline seq_t _my_binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* ----- sequence function ----- */

seq_t a(idx_t n)
{
    /* 1. locate the row containing position n
       cumulative index after row r is cum + len(row r). */
    idx_t cum = 0;
    idx_t row = 0;
    while (cum + _row_len(row) <= n) {
        cum += _row_len(row);
        ++row;
    }

    /* 2. offset inside that row */
    idx_t offset = n - cum;          /* 0‑based offset within the row */

    /* 3. determine which sub‑segment (k) contains this offset */
    idx_t m = row / 2;               /* largest possible k */
    idx_t k = 0;
    while (k <= m) {
        idx_t seg_len = row - 2 * k + 1;   /* length of segment k */
        if (offset < seg_len)
            break;
        offset -= seg_len;
        ++k;
    }

    /* 4. compute value: B(row,k) * binom(row-2k, offset) */
    seq_t B = _my_binom(row, 2 * k) * _odd_factorial(k);
    seq_t bin = _my_binom(row - 2 * k, offset);
    return B * bin;
}
