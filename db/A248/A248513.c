/*
 * A248513  Rectangular array by antidiagonals: the dispersion of A181155
 *          ("odious numbers").
 * https://oeis.org/A248513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n-th element of the dispersion of odious numbers
 *            A181155, enumerated by antidiagonals
 * Range:     u8 n <= 29, u16 n <= 61, u32 n <= 399, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Utility: count even‑parity numbers in [0, n] (n >= 0) */
static seq_t count_even(seq_t n)
{
    if (n == (seq_t)0)
        return (seq_t)1;         /* only 0 */

    /* find highest power of two <= n */
    seq_t high = (seq_t)1;
    while (high <= n) high <<= 1;
    high >>= 1;                  /* high <= n < 2*high */

    seq_t rest = n - high;
    seq_t even_high = (high == (seq_t)1) ? (seq_t)1 : (high >> 1); /* evens below high */

    seq_t odd_rest = (rest + (seq_t)1) - count_even(rest);          /* odds in [0,rest] */
    return even_high + odd_rest;
}

/* Count numbers in S (= those where (x-1) has odd popcount) up to x */
static seq_t count_S(seq_t x)
{
    if (x == (seq_t)0)
        return (seq_t)0;
    return x - count_even(x - (seq_t)1);
}

/* Find the k-th even‑parity number (0‑indexed) */
static seq_t kth_even(seq_t k)   /* k >= 0 */
{
    seq_t lo = (seq_t)0, hi = (seq_t)1;
    while (count_even(hi) <= k) hi <<= 1;

    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (count_even(mid) > k)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Find the k-th odd‑parity number (1‑indexed) */
static seq_t kth_odd(seq_t k)   /* k >= 1 */
{
    seq_t lo = (seq_t)1, hi = (seq_t)2;
    while (count_S(hi) < k) hi <<= 1;

    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (count_S(mid) >= k)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Helper: triangular numbers T(d) = (d-1)*d/2 (d >= 1) */
static idx_t triangular(idx_t d)
{
    return (idx_t)(((unsigned __int128)(d - 1) * d) / 2);
}

/* Find the diagonal index d such that (d-1)*d/2 >= n */
static idx_t diagonal(idx_t n)
{
    idx_t lo = (idx_t)1, hi = (idx_t)1;
    while (triangular(hi) < n) hi <<= 1;

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (triangular(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* position (i,j) in the array */
    idx_t d = diagonal(n);                     // diagonal
    idx_t base = triangular(d - 1);            // elements before this diagonal
    idx_t offset = n - base - (idx_t)1;        // 0‑based offset on the diagonal
    idx_t i = offset + (idx_t)1;               // row
    idx_t j = d - i;                           // column (>=1)

    /* build t(i) : i‑th element of the t sequence = evil(i-1)+1 */
    seq_t ev = kth_even((seq_t)(i - (idx_t)1));/* i-1  is 0‑based */
    seq_t val = ev + (seq_t)1;                 /* t(i) */

    /* apply s j-1 times */
    for (idx_t step = (idx_t)2; step <= j; ++step)
        val = kth_odd(val);                    /* val is used as index */

    return val;
}
