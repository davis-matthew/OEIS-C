/*
 * A325774  Rectangular array: row n shows the number of parts in all
 *          partitions of n that are == k (mod 5), for k = 0, 1, 2, 3, 4.
 * https://oeis.org/A325774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 171, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = total number of parts ≡ k (mod 5) among all partitions of
 *               r, where r = (n-1)/5 + 1  (1‑based row number)
 *               and k = (n-1) % 5    (column 0…4)
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build.  No other includes are needed. */

static seq_t *p = NULL;          /* p[n] = number of partitions of n */
static seq_t *r[5] = {NULL};     /* r[k][n] = count of parts ≡ k (mod 5) in partitions of n */
static size_t cap = 0;           /* maximum n for which data have been built */

/* Build/rebuild all data up to 'up' (inclusive).  The function frees previous
 * arrays and allocates fresh ones.  Building is O(up²) which is fine for the
 * expected limits (up ≈ 2000 for n ≈ 10000). */
static void build_up_to(idx_t up)
{
    /* free any previous data */
    if (p) free(p);
    for (int k = 0; k < 5; ++k) {
        if (r[k]) free(r[k]);
    }

    size_t sz = (size_t)up + 1;          /* number of entries: 0..up */

    p = (seq_t *)calloc(sz, sizeof(seq_t));
    for (int k = 0; k < 5; ++k) {
        r[k] = (seq_t *)calloc(sz, sizeof(seq_t));
    }

    /* base: empty partition */
    p[0] = (seq_t)1;

    /* DP over part sizes */
    for (idx_t m = 1; m <= up; ++m) {
        int km = (int)(m % 5);          /* residue of the part size */
        for (idx_t n = m; n <= up; ++n) {
            seq_t add = p[n - m];       /* partitions of n-m to which we add part m */

            p[n] += add;
            for (int k = 0; k < 5; ++k) {
                r[k][n] += r[k][n - m];
                if (k == km) {
                    r[k][n] += add;      /* each new part contributes one count */
                }
            }
        }
    }

    cap = up;
}

/* The requested OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* offset starts at 1 */

    /* Determine row and column corresponding to the linear index n. */
    idx_t row = (n - 1) / 5 + 1;       /* 1‑based row number */
    int   k   = (int)((n - 1) % 5);    /* column 0…4 */

    /* Ensure data up to required row have been computed. */
    if ((size_t)row > cap) {
        build_up_to(row);
    }

    return r[k][row];
}
