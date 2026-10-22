/*
 * A397496  Triangle read by rows: T(n, k) is the concatenation of numbers
 *          from n down to n + 1 - k, n >= k >= 1.
 * https://oeis.org/A397496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl, easy
 * Formula:   a(n) = concat_{i=n+k-1}^{n} i for k = n - (r-1)r/2, r = ceil(
 *            (-1 + sqrt(1+8n)) / 2 ) (row index)
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 54, u64 n <= 119, u128 n <= 323,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* count decimal digits of a positive integer */
static idx_t num_digits_idx(idx_t x)
{
    idx_t d = 0;
    while (x > 0) {
        ++d;
        x /= 10;
    }
    return d;
}

/* 10^d as seq_t */
static seq_t pow10_seq(idx_t d)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < d; ++i) {
        res *= (seq_t)10;
    }
    return res;
}

/* find the row index r such that r*(r+1)/2 >= n */
static idx_t row_of(idx_t n)
{
    /* simple incremental search – fast enough for n up to ≈10⁶ */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)n;   /* a(1) = 1, also safe for n = 0 (not used) */
    }

    idx_t r = row_of(n);

    /* terms before this row */
    idx_t prev = r * (r - 1) / 2;

    /* column position (1‑based) */
    idx_t k = n - prev;          /* 1 ≤ k ≤ r */

    /* starting integer in the concatenation (inclusive) */
    idx_t start = r + 1 - k;     /* r, r-1, ..., start */

    seq_t result = (seq_t)0;
    for (idx_t i = r; ; --i) {
        idx_t d = num_digits_idx(i);
        seq_t mult = pow10_seq(d);
        result = result * mult + (seq_t)i;
        if (i == start) {
            break;
        }
    }
    return result;
}
