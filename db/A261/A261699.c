/*
 * A261699  Triangle read by rows: T(n,k), n >= 1, k >= 1, in which column k
 *          lists positive terms interleaved with k-1 zeros, starting in row
 *          k(k+1)/2. If k is odd the positive terms of column k are k's,
 *          otherwise if k is even the positive terms of column k are the odd
 *          numbers greater than k in increasing or...
 * https://oeis.org/A261699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A261699: Triangle read by rows, flattened into sequence a(n).
   The sequence is obtained by reading the triangle
        T(n,k) = { k                 if k is odd and n = k(k+1)/2 + k·m
                   k+1+2m            if k is even and n = k(k+1)/2 + k·m
                 0                  otherwise }
   where m = 0,1,2,...  and k >= 1, n >= 1.
   Row n contains A003056(n) entries.  The flattened sequence a(n) is
   the n-th entry in row-major order.  The implementation below
   computes the position’s row and column on-the-fly and then applies
   the definition.  It works for any unsigned seq_t type (8/16/32/64/128 bits
   or arbitrary‑precision) and for any idx_t type (uint64_t).  No external
   libraries beyond the allowed ones are required.  */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);      /* hi ≥ sqrt(2^64-1) */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;                 /* safe: mid ≤ 2^32 */
        if (sq <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Length of row r in the triangle:
   A003056(r) = floor((sqrt(8r+1)-1)/2)  */
static idx_t row_len(idx_t r)
{
    return (isqrt_u64(8 * r + 1) - 1) / 2;
}

/* Cumulative array: cum[i] = total number of entries in rows 1..i.
   cum[0] is defined as 0. */
static idx_t *cum = NULL;          /* allocated on first use */
static size_t cum_cnt = 0;         /* number of elements stored (>=1) */
static size_t cum_cap = 0;         /* allocated capacity */

/* Ensure that cum covers all rows up to an index providing at least
   target_index entries.  */
static void ensure_cumulative(idx_t target_index)
{
    if (cum == NULL) {                      /* first use */
        cum_cap = 64;
        cum = (idx_t *)malloc(cum_cap * sizeof(idx_t));
        if (!cum) abort();                  /* out of memory */
        cum[0] = 0;                          /* before row 0 */
        cum_cnt = 1;                         /* one element stored */
    }
    while (cum_cnt <= 1 || cum[cum_cnt - 1] < target_index) {
        idx_t r = cum_cnt;                   /* next row number */
        idx_t len = row_len(r);
        if (cum_cnt >= cum_cap) {            /* grow storage if necessary */
            size_t new_cap = cum_cap * 2;
            idx_t *new_arr = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
            if (!new_arr) abort();
            cum = new_arr;
            cum_cap = new_cap;
        }
        cum[cum_cnt] = cum[cum_cnt - 1] + len;
        cum_cnt++;
    }
}

/* Triangle entry at (n,k) according to the definition.  */
static inline seq_t triangle_value(idx_t n, idx_t k)
{
    idx_t start = k * (k + 1) / 2;
    if (n < start) return 0;
    idx_t diff = n - start;
    if (diff % k != 0) return 0;
    idx_t p = diff / k;
    if (k & 1) {
        return (seq_t)k;
    } else {
        return (seq_t)(k + 1 + 2 * p);
    }
}

/* Public sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* out of range protection */
    ensure_cumulative(n);

    /* Binary search for the row containing index n. */
    idx_t low = 1, high = cum_cnt - 1, row = 1;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (cum[mid] >= n) {
            row = mid;
            if (mid == 0 || cum[mid - 1] < n) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    idx_t prev = cum[row - 1];
    idx_t col0 = n - prev - 1;      /* zero‑based column within the row */
    idx_t col = col0 + 1;           /* one‑based column index */
    return triangle_value(row, col);
}
