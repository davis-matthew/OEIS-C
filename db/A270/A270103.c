/*
 * A270103  Array read by antidiagonals: T(n, k) is the sum of the integer
 *          part of the n-th roots of natural numbers less than k.
 * https://oeis.org/A270103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = sum_{j=0..k} floor(j^(1/n)) read by antidiagonals. The
 *            sequence is computed by mapping the index n to the coordinates
 *            (row = r, col = k) of the anti‑diagonal array, then evaluating
 *            T(r,k) = (1+k)*floor(k^(1/r)) - sum_{j=1..floor(k^(1/r))} j^r
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if base^exp <= limit, avoiding overflow. */
static bool pow_leq(idx_t base, idx_t exp, idx_t limit)
{
    seq_t prod = 1;
    for (idx_t i = 0; i < exp; ++i) {
        if (prod != 0) {
            if (limit / (uint64_t)prod < (uint64_t)base)
                return false;
        }
        prod *= (seq_t)base;
        if (prod > (seq_t)limit)
            return false;
    }
    return prod <= (seq_t)limit;
}

/* Compute base^exp exactly, with small base and exp. */
static seq_t pow_exact(idx_t base, idx_t exp)
{
    seq_t prod = 1;
    for (idx_t i = 0; i < exp; ++i)
        prod *= (seq_t)base;
    return prod;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0, return 0 */

    /* Find diagonal d such that d*(d+1)/2 >= n. */
    idx_t d = 0, cum = 0;
    while (cum < n) {
        ++d;
        cum += d;
    }
    idx_t prev = cum - d;             /* total terms before this diagonal */
    idx_t offset = n - prev - 1;      /* 0‑based offset inside diagonal */

    idx_t r = offset + 1;             /* row index */
    idx_t k = d - offset;             /* column index */

    /* Compute m = floor(k^(1/r)). */
    idx_t lo = 1, hi = k;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (pow_leq(mid, r, k))
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t m = lo;

    /* Sum_{j=1..m} j^r */
    seq_t sum = 0;
    for (idx_t j = 1; j <= m; ++j)
        sum += pow_exact(j, r);

    /* T(r,k) = (1+k)*m - sum. */
    seq_t term = (seq_t)(1 + k) * (seq_t)m - sum;
    return term;
}
