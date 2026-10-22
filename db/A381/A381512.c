/*
 * A381512  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) = (2*n+k)!/k! * [x^(2*n+k)] sinh(x)^k.
 * https://oeis.org/A381512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A381512 – square array A(n,k) read by antidiagonals.
 *
 *  Recurrence (k>1):
 *      A(n,k) = k^2 * A(n-1,k) + A(n,k-2)
 *  Base values:
 *      A(0,0) = 1
 *      A(0,j) = 1     for j≥0
 *      A(n,0) = 0     for n>0
 *      A(n,1) = 1     for n≥0
 *
 *  The sequence a(n) is the entry read by antidiagonals:
 *      a(n) = A(i,j)  where i+j = s = ⌊(√(8n+1)-1)/2⌋,
 *      i     = n - s(s+1)/2,
 *      j     = s - i.
 *
 *  The implementation builds the table A(n,k) incrementally,
 *  storing the values up to the current highest diagonal s.
 *  All arithmetic uses the given type seq_t; idx_t is unsigned 64‑bit.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* auxiliary: integer square root of a 64‑bit unsigned integer */
static inline idx_t isqrt_u64(idx_t n)
{
    /* binary search on [0, 2^32] – fits into 64‑bit */
    idx_t lo = 0, hi = (((idx_t)1) << 32);
    while (lo < hi) {
        idx_t mid = (lo + hi) / 2;
        if (mid != 0 && mid > n / mid)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo - 1;
}

/* convert linear index n to coordinates (i,j) of the antidiagonal */
static inline void idx_to_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t t = 8 * n + 1;
    idx_t r = isqrt_u64(t);
    idx_t s = (r - 1) >> 1;            /* floor((sqrt(8n+1)-1)/2) */

    /* correct possible off‑by‑one due to sqrt rounding */
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;
    while (s * (s + 1) / 2 > n)
        --s;

    idx_t tri = s * (s + 1) / 2;
    *i = n - tri;
    *j = s - *i;
}

/* global cache of computed A(n,k) values */
static seq_t *A_flat = NULL;
static idx_t max_s   = 0;

/* rebuild the table up to diagonal s (inclusive) */
static void rebuild_table(idx_t s)
{
    size_t size = (size_t)(s + 1) * (s + 1);
    seq_t *newA = (seq_t *)malloc(size * sizeof(seq_t));
    if (!newA) return;  /* silently abort if allocation fails */

    for (idx_t i = 0; i <= s; ++i) {
        for (idx_t j = 0; j <= s; ++j) {
            seq_t val;
            if (i == 0 && j == 0)
                val = 1;                  /* A(0,0) = 1 */
            else if (i == 0)
                val = 1;                  /* A(0,j) = 1 for all j≥0 */
            else if (j == 0)
                val = 0;                  /* A(n,0) = 0 for n>0 */
            else if (j == 1)
                val = 1;                  /* A(n,1) = 1 */
            else {
                seq_t mult = (seq_t)j * (seq_t)j *
                              newA[(size_t)(i - 1) * (s + 1) + j];
                seq_t add  = newA[(size_t)i * (s + 1) + (j - 2)];
                val = mult + add;
            }
            newA[(size_t)i * (s + 1) + j] = val;
        }
    }

    free(A_flat);
    A_flat = newA;
    max_s   = s;
}

/* public entry point */
seq_t a(idx_t n)
{
    idx_t i, j;
    idx_to_coords(n, &i, &j);
    idx_t s = i + j;

    /* lazily initialise or extend the table */
    if (!A_flat || s > max_s)
        rebuild_table(s);

    return A_flat[(size_t)i * (s + 1) + j];
}
