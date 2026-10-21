/*
 * A290458  Triangle formed by reading the triangle of Eulerian numbers
 *          (A173018) mod 8.
 * https://oeis.org/A290458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A173018(m,k) mod 8 where (m,k) is the unique pair such
 *            that n = m*(m+1)/2 + k and 0 ≤ k ≤ m. Eulerian numbers satisfy:
 *            // A(0,0) = 1 // A(m,0) = 1, A(m,m) = 0 // A(m,k) = (m-k) *
 *            A(m-1,k-1) + (k+1) * A(m-1,k) for 1≤k≤m-1 Reading those triangle
 *            rows consecutively gives the sequence
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Integer square‐root floor for 0 ≤ x < 2^64. */
static idx_t isqrt_u64(idx_t x)
{
    unsigned __int128 r = 0;
    for (int shift = 32; shift >= 0; --shift) {
        unsigned __int128 t = r | ((unsigned __int128)1 << shift);
        if (t * t <= x) r = t;
    }
    return (idx_t)r;
}

/* Row index r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
static idx_t row_of(idx_t n)
{
    idx_t s = isqrt_u64(1 + 8 * n);   /* floor(sqrt(8n+1)) */
    return (s - 1) / 2;
}

/* Flattened Eulerian triangle (mod 8), stored rowwise. */
static uint8_t *flatt = NULL;         /* data storage                */
static size_t   cap   = 0;            /* current allocation size     */
static idx_t    done  = 0;            /* largest row fully computed */

/* Previous row, used to generate the next one. */
static uint8_t *prev_row = NULL;
static size_t   prev_len = 0;

/* Ensure rows 0 … needed are computed and stored. */
static void ensure_rows(idx_t needed)
{
    /* First time: produce row 0. */
    if (flatt == NULL) {
        cap = 1;
        flatt = (uint8_t *)malloc(cap);
        if (!flatt) abort();
        flatt[0] = 1;                 /* A(0,0) = 1 */
        prev_row = (uint8_t *)malloc(1);
        if (!prev_row) abort();
        prev_row[0] = 1;
        prev_len = 1;
        done = 0;
    }

    if (done >= needed) return;

    for (idx_t r = done + 1; r <= needed; ++r) {
        size_t len = (size_t)r + 1;      /* row length */
        uint8_t *cur = (uint8_t *)malloc(len);
        if (!cur) abort();

        /* Compute this row using the recurrence. */
        for (size_t k = 0; k <= r; ++k) {
            uint8_t val;
            if (k == 0) {
                val = 1;                 /* A(r,0) = 1 */
            } else if (k == r) {
                val = 0;                 /* A(r,r) = 0 */
            } else {
                int term = (int)((r - k) * prev_row[k - 1]
                               + (k + 1) * prev_row[k]);
                val = (uint8_t)(term & 7);   /* mod 8 */
            }
            cur[k] = val;
        }

        /* Place row into the flattened array. */
        idx_t start = r * (r + 1) / 2;        /* first index of this row */
        idx_t required = start + len;         /* total needed size */

        if (required > cap) {
            size_t new_cap = cap ? cap : 128;
            while (new_cap < required) new_cap <<= 1;
            uint8_t *tmp = (uint8_t *)realloc(flatt, new_cap);
            if (!tmp) abort();
            flatt = tmp;
            cap = new_cap;
        }

        memcpy(flatt + start, cur, len);

        /* Prepare for next row. */
        free(prev_row);
        prev_row = cur;
        prev_len = len;
    }
    done = needed;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);       /* row containing the index n */
    ensure_rows(r);            /* make sure all needed rows exist */
    uint8_t val = flatt[(size_t)n];
    return (seq_t)val;
}
