/*
 * A380834  First column of Kimberling's ESC array.
 * https://oeis.org/A380834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth entry in the first column of Kimberling's ESC
 *            array. The ESC array is defined by: ESC[i,1] = a(i) ESC[i,2] =
 *            floor(phi*ESC[i,1]) + (i mod 2) ESC[i,j] = ESC[i,j-1] +
 *            ESC[i,j-2] (j >= 3) ESC[i,1] is chosen to be the smallest
 *            positive integer not already appearing in any entry of row...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* idx_t and seq_t are supplied by the build system. */
extern idx_t _floor_isqrt_u64(idx_t n);          /* Not used; we implement own sqrt. */

/* ----- auxiliary integer‑sqrt for 128‑bit values ----- */
static inline idx_t isqrt_u128(__int128 x)
{
    idx_t r = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t bit = 1ULL << i;
        idx_t cand = r | bit;
        __int128 sq = (__int128)cand * cand;
        if (sq <= x) r = cand;
    }
    return r;
}

/* floor(phi * m) using only integer arithmetic.
   phi = (1 + sqrt(5)) / 2  */
static inline idx_t floor_phi_mul(idx_t m)
{
    __int128 val = (__int128)5 * m * m;      /* 5*m² fits in 128 bits */
    idx_t s = isqrt_u128(val);              /* floor(sqrt(5*m²))   */
    return (m + s) / 2;                     /* floor((m + s)/2)   */
}

/* ----- static state for memoisation ----- */
static seq_t *seq_vals  = NULL;          /* 1‑based array of sequence values */
static size_t seq_cap  = 0;              /* allocated capacity of seq_vals   */
static size_t seq_len  = 0;              /* number of values already computed */
static unsigned char *used = NULL;       /* boolean array of used integers       */
static size_t used_cap   = 0;            /* capacity of used array              */
static idx_t next_candidate = 1;         /* smallest unused integer after seq_len rows */

/* Utility to ensure the sequence array can hold 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 32;
    while (new_cap < need) new_cap *= 2;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!seq_vals) exit(EXIT_FAILURE);
    seq_cap = new_cap;
}

/* Utility to ensure the used array can index up to 'need' (inclusive). */
static void ensure_used_cap(idx_t need)
{
    if ((idx_t)used_cap > need) return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while ((idx_t)new_cap <= need) new_cap *= 2;
    used = (unsigned char *)realloc(used, new_cap);
    if (!used) exit(EXIT_FAILURE);
    memset(used + used_cap, 0, new_cap - used_cap);
    used_cap = new_cap;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    /* Initialise static structures on first call */
    if (seq_vals == NULL) {
        ensure_seq_cap(64);                 /* initial capacity */
        ensure_used_cap((idx_t)2000000);    /* big enough for all n up to 10 000 */
    }

    /* If already computed, return the stored value */
    if ((size_t)n < seq_len + 1) return seq_vals[(size_t)n];

    /* Compute new rows up to n */
    for (size_t i = seq_len + 1; i <= (size_t)n; ++i) {
        /* The next element of the sequence is the current smallest
           unused integer after the previous rows have been processed */
        seq_vals[i] = (seq_t)next_candidate;

        /* Produce the i‑th row of the ESC array and mark all numbers
           that appear (up to the current used array capacity). */
        idx_t a_val = (idx_t)seq_vals[i];                     /* ESC[i,1] */
        idx_t b_val = floor_phi_mul(a_val) + ((i & 1) ? 1 : 0); /* ESC[i,2] */

        idx_t a = a_val, b = b_val;
        for (;;) {
            if (a < (idx_t)used_cap) used[a] = 1; else break;
            if (b < (idx_t)used_cap) used[b] = 1; else break;
            idx_t c = a + b;
            a = b;
            b = c;
            if (a >= (idx_t)used_cap) break;      /* subsequent values larger */
        }

        /* Update the smallest unused integer for the next iteration */
        while ((idx_t)next_candidate < used_cap && used[next_candidate]) {
            ++next_candidate;
        }
    }

    seq_len = (size_t)n;
    return seq_vals[(size_t)n];
}
