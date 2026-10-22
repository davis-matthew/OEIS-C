/*
 * A316938  Triangle read by rows formed using Pascal's rule except that n-th
 *          row begins and ends with Fibonacci(n+4).
 * https://oeis.org/A316938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the (n‑th) element of a triangle read by rows.
    Row r (r ≥ 0) has r+1 elements.
    The first and last elements of row r are Fibonacci(r+4).
    Interior elements satisfy Pascal's rule:
        t(r,k) = t(r‑1,k‑1) + t(r‑1,k)   (1 ≤ k ≤ r‑1).
    The sequence is the flattening of the triangle:
        a(0) = 3; a(1) = 5; a(2) = 5; … */

/*  The function memoises computed values so it works for arbitrary
    call order and for any n ≥ 0.  It uses only integer arithmetic
    compatible with all supported seq_t and idx_t types.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/fib.h"

static seq_t *cache_seq      = NULL;   /* linear storage of all elements          */
static size_t cache_cap      = 0;
static idx_t  cache_len      = 0;       /* number of elements stored               */

static idx_t *row_start      = NULL;   /* start index of each row in cache_seq    */
static size_t row_start_cap   = 0;
static idx_t  row_count      = 0;       /* number of rows generated                */

/* Ensure cache_seq has room for at least 'needed' elements. */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache_seq, new_cap * sizeof(seq_t));
    if (!newp) abort();
    cache_seq = newp;
    cache_cap = new_cap;
}

/* Ensure row_start has capacity for at least 'needed' rows. */
static void ensure_row_start_capacity(size_t needed)
{
    if (needed <= row_start_cap) return;
    size_t new_cap = row_start_cap ? row_start_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    idx_t *newp = (idx_t *)realloc(row_start, new_cap * sizeof(idx_t));
    if (!newp) abort();
    row_start = newp;
    row_start_cap = new_cap;
}

/* Compute a(n).  The function is memoised; it generates rows of
   the triangle on demand. */
seq_t a(idx_t n)
{
    /* generate rows until we have enough elements */
    while (cache_len <= n) {
        idx_t r = row_count;          /* 0‑based row number */
        idx_t row_len = r + 1;

        ensure_seq_capacity(cache_len + row_len);
        ensure_row_start_capacity(row_count + 1);

        /* store start index of this row */
        row_start[r] = cache_len;

        /* compute row elements */
        for (idx_t k = 0; k < row_len; ++k) {
            seq_t val;
            if (k == 0 || k == r) {
                /* first and last elements are Fibonacci(r+4) */
                val = fib(r + 4);
            } else {
                /* sum of two values from the previous row */
                idx_t prev_base = row_start[r - 1];
                val = cache_seq[prev_base + (k - 1)]
                    + cache_seq[prev_base + k];
            }
            cache_seq[cache_len + k] = val;
        }
        cache_len += row_len;
        row_count++;
    }
    return cache_seq[n];
}
