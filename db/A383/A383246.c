/*
 * A383246  Positive integers without the digit 0 such that every even digit
 *          except the rightmost is immediately followed by a smaller digit.
 * https://oeis.org/A383246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate all positive integers without digit 0 such that every
 * even digit except the rightmost is immediately followed by a smaller digit. */

#include <stddef.h>            /* size_t */

/* static storage for the sequence values; 20 000 elements is more than
 * sufficient for the first 10 000 terms. */
static seq_t seq_cache[20000];
static size_t cache_size = 0;  /* current number of terms stored */
static int next_length   = 1;  /* next number of digits to generate */

/* Append a new value to the cache.  We assume that cache_size < 20000. */
static void push_cache(seq_t val)
{
    if (cache_size < 20000) {
        seq_cache[cache_size++] = val;
    }
}

/* Recursively generate all valid numbers with a given total length.
 * pos   : current position (0 = most significant digit)
 * cur   : numeric value built so far
 * prev  : previous digit; 0 only for the first call
 * len   : total number of digits of the target numbers
 */
static void gen_len_rec(int len, int pos, seq_t cur, int prev)
{
    if (pos == len) {                /* finished a number */
        push_cache(cur);
        return;
    }
    for (int d = 1; d <= 9; ++d) {    /* digits 1–9 only */
        if (pos > 0 && (prev % 2 == 0) && d >= prev) {
            /* even previous digit must be followed by a smaller one */
            continue;
        }
        gen_len_rec(len, pos + 1, cur * 10 + d, d);
    }
}

/* Return the n-th term of sequence A383246 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;             /* safety check, real sequence starts at 1 */
    }
    if (n <= cache_size) {
        return seq_cache[n - 1];
    }

    /* Generate additional terms until we have at least n terms. */
    int len = next_length;
    while (cache_size < n) {
        gen_len_rec(len, 0, 0, 0);
        ++len;
    }
    next_length = len;                /* prepare for next call */

    return seq_cache[n - 1];
}
