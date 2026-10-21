/*
 * A249722  Numbers n such that there is a multiple of 4 on row n of Pascal's
 *          triangle with property that all multiples of 9 on the same row (if
 *          they exist) are larger than it.
 * https://oeis.org/A249722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th row index of Pascal's triangle such that *
 *            there is a multiple of 4 in that row and, if a multiple of 9
 *            also exists, * the smallest multiple of 4 occurs before the
 *            smallest multiple of 9. * * This implementation uses Kummer's
 *            theorem to decide divisibility by 2^2 * (i.e. 4) and 3...
 * Range:     u8 n <= 155, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Utility: pool of computed binomial coefficient digit sums. */
static idx_t *s2 = NULL; /* sum of binary digits of i */
static idx_t *s3 = NULL; /* sum of base‑3 digits of i */
static idx_t  s2size = 0;
static idx_t  s3size = 0;

/* Ensure that digit sums are available for all values up to n. */
static void ensure_digit_sums(idx_t n)
{
    if (n < s2size) return;
    idx_t newsize = n + 1;
    idx_t *tmp2 = (idx_t *)realloc(s2, newsize * sizeof(idx_t));
    idx_t *tmp3 = (idx_t *)realloc(s3, newsize * sizeof(idx_t));
    if (!tmp2 || !tmp3) { /* allocation failed – abort */
        exit(1);
    }
    s2 = tmp2;
    s3 = tmp3;
    for (idx_t i = s2size; i < newsize; ++i) {
        s2[i] = s2[i >> 1] + (i & 1);
        s3[i] = s3[i / 3] + (i % 3);
    }
    s2size = s3size = newsize;
}

/* Test whether the row r satisfies the property. */
static bool row_has_property(idx_t r)
{
    ensure_digit_sums(r);           /* sums up to r need for n-n-k */
    idx_t k4 = 0, k9 = 0;           /* 0 means not found yet */
    for (idx_t k = 1; k < r; ++k) {
        /* v2(C(r,k)) = s2(k) + s2(r-k) - s2(r)  (base 2: denominator 1) */
        idx_t v2 = s2[k] + s2[r - k] - s2[r];
        if (v2 >= 2 && !k4) k4 = k;

        /* v3(C(r,k)) = (s3(k) + s3(r-k) - s3(r)) / 2  (base 3) */
        idx_t v3 = (s3[k] + s3[r - k] - s3[r]) >> 1;
        if (v3 >= 2 && !k9) k9 = k;

        if (k4 && k9) break; /* found both smallest positions */
    }

    if (!k4) return false;          /* must have a multiple of 4 */
    if (!k9) return true;           /* no multiple of 9 => property holds */

    /* property: first 4 must come before first 9; equality means fail */
    return k4 < k9;
}

/* Cache of the sequence values. */
static seq_t *seq_buf = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static idx_t last_row = 0;          /* last row examined */

/* Append a value to the cache. */
static void seq_append(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t newcap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_buf, newcap * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failed */
        seq_buf = tmp;
        seq_cap = newcap;
    }
    seq_buf[seq_len++] = val;
}

/* External linkage: the required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; below offset is harmless */

    /* Compute rows until we have at least n terms. */
    while (seq_len < n) {
        ++last_row;
        if (row_has_property(last_row))
            seq_append((seq_t)last_row);
    }
    return seq_buf[n - 1];
}
