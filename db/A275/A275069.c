/*
 * A275069  Number A(n,k) of set partitions of [n] such that i-j is a multiple
 *          of k for all i,j belonging to the same block; square array A(n,k),
 *          n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A275069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = product_{i=0}^{k-1} A000110(floor((n+i)/k)) where
 *            A000110 is the Bell number. The table A(n,k) is read by
 *            antidiagonals, so a(n) returns the n‑th element with indices
 *            (n,k) satisfying n + k = d for the d‑th antidiagonal
 * Range:     u8 n <= 42, u16 n <= 75, u32 n <= 168, u64 n <= 403,
 *            u128 n <= 1032, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*--- Bell number cache -----------------------------------------------------*/
static seq_t *bell = NULL;   /* bell[i] stores Bell(i) */
static size_t bell_len = 0;  /* number of stored Bell numbers */
static size_t bell_cap = 0;  /* allocated capacity */

/* Ensure that Bell numbers up to index m are precomputed */
static void ensure_bell_upto(idx_t m)
{
    /* initialise with Bell(0) = 1 if not yet done */
    if (bell_len == 0) {
        bell_cap = 4;
        bell = (seq_t*)malloc(bell_cap * sizeof(seq_t));
        if (!bell) abort();
        bell[0] = (seq_t)1;
        bell_len = 1;
    }

    /* Compute missing Bell numbers */
    while (bell_len <= m) {
        idx_t n = bell_len - 1;               /* current n for B_{n+1} */
        seq_t sum = 0;
        seq_t binom = 1;                       /* C(n,0) */

        for (idx_t k = 0; (idx_t)k <= n; ++k) {
            sum += binom * bell[(size_t)k];
            if (k < n)
                binom = (binom * (seq_t)(n - k)) / (seq_t)(k + 1);
        }

        /* Grow array if necessary */
        if (bell_len >= bell_cap) {
            size_t new_cap = bell_cap ? bell_cap * 2 : 4;
            seq_t *tmp = (seq_t*)realloc(bell, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            bell = tmp;
            bell_cap = new_cap;
        }

        bell[bell_len++] = sum;
    }
}

/* Convert a flat index to (n, k) on antidiagonals:
   (n,k) with n + k = s, offset 0 <= n <= s.              */
static void antidiagonal_coords(idx_t idx, idx_t *n, idx_t *k)
{
    idx_t s = 0;
    /* Find largest s such that s*(s+1)/2 <= idx */
    while (((s + 1) * (s + 2)) / 2 <= idx)
        ++s;
    idx_t prev_total = s * (s + 1) / 2;
    idx_t offset = idx - prev_total;   /* position within diagonal s */
    *n = offset;
    *k = s - offset;
}

/*--- Entry point -----------------------------------------------------------*/
seq_t a(idx_t idx)
{
    idx_t n, k;
    antidiagonal_coords(idx, &n, &k);

    /* Empty product for k = 0 */
    if (k == 0) return (seq_t)1;

    /* Find maximum floor((n+i)/k) needed */
    idx_t max_floor = (n + k - 1) / k;
    ensure_bell_upto(max_floor);

    seq_t prod = 1;
    for (idx_t i = 0; i < k; ++i) {
        idx_t m = (n + i) / k;
        prod *= bell[(size_t)m];
    }
    return prod;
}
