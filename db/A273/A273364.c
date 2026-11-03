/*
 * A273364  Numbers n such that the decimal number concat(9,n) is a square.
 * https://oeis.org/A273364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 1056, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 9831 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that the decimal number concat(9,n) is a square.
   We generate the sequence by scanning over digit lengths d.
   For each d we test all m with 9·10^d ≤ m² < 10^(d+1); the value
   n = m² – 9·10^d is kept iff it has exactly d decimal digits. */

/* Include common helpers. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Ceil(sqrt(x)) using floor_sqrt. */
static inline seq_t ceil_sqrt(seq_t x)
{
    seq_t s = floor_sqrt(x);
    seq_t sq = s * s;
    return (sq < x) ? s + (seq_t)1 : s;
}

/* Cache of powers of 10.  pow10_rec[i] = 10^i. */
static seq_t *pow10_rec   = NULL;
static size_t  pow10_len  = 0;
static size_t  pow10_cap  = 0;

/* Ensure that pow10_rec contains powers up to exponent 'd'. */
static void ensure_pow10(idx_t d)
{
    if (pow10_len > d) return;

    size_t new_cap = pow10_cap ? pow10_cap * 2 : 4;
    while (new_cap <= (size_t)d) new_cap <<= 1;
    pow10_rec = mem_resize(seq_t, pow10_rec, new_cap);

    for (idx_t i = pow10_len; i <= d; ++i) {
        if (i == 0) pow10_rec[0] = (seq_t)1;
        else        pow10_rec[i] = pow10_rec[i - 1] * (seq_t)10;
    }
    pow10_len = d + 1;
    pow10_cap = new_cap;
}

/* Cached sequence values.  seq_arr holds the terms in increasing order. */
static seq_t *seq_arr   = NULL;
static size_t  seq_len  = 0;      /* number of stored terms             */
static size_t  seq_cap  = 0;      /* allocated capacity                 */

/* Highest digit length that has been fully processed. */
static idx_t   last_done = 0;

/* Generate sequence terms until at least 'n' terms exist. */
static void ensure_terms(idx_t n)
{
    /* Continue until we have enough terms. */
    while (seq_len < (size_t)n) {
        idx_t d = last_done + 1;                /* next digit length to check */
        ensure_pow10(d);

        seq_t pow10_d       = pow10_rec[d];
        seq_t pow10_d_plus1 = pow10_d * (seq_t)10;
        seq_t nine          = (seq_t)9;

        seq_t lower = ceil_sqrt(nine * pow10_d);
        seq_t upper = floor_sqrt(pow10_d_plus1 - (seq_t)1);
        seq_t lower_n = pow10_d / (seq_t)10;     /* 10^(d-1)  */

        for (seq_t m = lower; m <= upper; ++m) {
            seq_t sq   = m * m;
            seq_t cand = sq - (nine * pow10_d);
            if (cand >= lower_n && cand < pow10_d) {
                append_candidate(&seq_arr, &seq_len, &seq_cap, cand);
            }
        }
        /* All candidates for digit length d have been processed. */
        last_done = d;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* defensive guard, offset is 1 */
    ensure_terms(n);
    return seq_arr[n - 1];
}
