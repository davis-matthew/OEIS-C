/*
 * A357187  First differences A357186 = "Take the k-th composition in standard
 *          order for each part k of the n-th composition in standard order,
 *          then add up everything.".
 * https://oeis.org/A357187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A357186(n + 1) - A357186(n) where A357186(n) = sum over
 *            parts p of the n‑th composition in standard order of
 *            floor_log2(p) + 1. The standard order is graded reverse
 *            lexicographic: for each integer m the compositions of m are
 *            listed by cuts encoded in binary, with the most significant bit
 *            c...
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* floor_log2 for a 64‑bit unsigned integer (x > 0) */
static inline idx_t floor_log2_idx(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1)
        ++r;
    return r;
}

/* Sum of floor_log2(parts) + 1 for the n‑th composition in standard order. */
static inline seq_t composition_sum(idx_t n)
{
    if (n == 0)
        return 0;                     /* empty composition */

    /* Determine the integer m whose compositions contain the n‑th one. */
    idx_t m = floor_log2_idx(n) + 1;   /* n ∈ [2^{m-1}, 2^m-1] */
    idx_t base = ((idx_t)1 << (m - 1));/* first index with sum m */
    idx_t offset = n - base;          /* 0‑based rank within this m */

    seq_t sum = 0;
    idx_t len = 1;                    /* current part length */
    idx_t bits = m - 1;               /* number of possible cut positions */

    for (idx_t pos = 0; pos < bits; ++pos) {
        idx_t bit = (offset >> (bits - 1 - pos)) & 1;
        if (bit) {                    /* cut after this element */
            sum += (seq_t)(floor_log2_idx(len) + 1);
            len = 1;
        } else {
            ++len;                    /* extend current part */
        }
    }

    /* add the last part */
    sum += (seq_t)(floor_log2_idx(len) + 1);
    return sum;
}

seq_t a(idx_t n)
{
    seq_t prev = composition_sum(n);
    seq_t cur  = composition_sum(n + 1);
    return cur - prev;
}
