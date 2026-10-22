/*
 * A345541  Numbers that are the sum of nine cubes in two or more ways.
 * https://oeis.org/A345541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that are the sum of nine non‑negative cubes in two or more ways
   (OEIS A345541).  The implementation enumerates all unordered representations
   of each integer as a sum of exactly nine positive cubes using a
   bounded knapsack DP.  It records all integers with at least two
   representations and returns the n‑th such integer.  The algorithm
   extends the range until it can answer the requested index.
   The DP uses counts capped at 2 to keep memory small and to speed
   the computation.  */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Helper: floor cubic root of a 64‑bit integer (inclusive).  */
static idx_t floor_cube_root(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

/* Compute terms up to at least 'n' terms.
   The function returns a dynamically allocated array of seq_t and its length.
   The caller is responsible for free'ing the array.  */
static void compute_terms(idx_t n, seq_t **out_terms, size_t *out_len)
{
    idx_t L = 200000;                 /* initial range */
    while (1) {
        size_t cap = (size_t)L + 1;     /* array size for sums 0..L */
        /* dp[parts][sum]  (parts 0..9)  stored as flat array */
        uint8_t *dp = (uint8_t *)calloc(10 * cap, 1);
        if (!dp) abort();               /* allocation failure */

        dp[0] = 1;                      /* 0 cubes sum to 0 */

        idx_t kmax = floor_cube_root(L);  /* largest cube value <= L */

        for (idx_t k = 1; k <= kmax; ++k) {
            seq_t cube = (seq_t)k * k * k;   /* k^3 fits in seq_t */
            size_t v = (size_t)cube;         /* <= L and fits in size_t */

            for (size_t s = v; s <= (size_t)L; ++s) {
                size_t prev_idx = s - v;
                for (int p = 1; p <= 9; ++p) {
                    uint8_t prev = dp[(p - 1) * cap + prev_idx];
                    if (prev) {
                        uint8_t cur = dp[p * cap + s];
                        int newv = cur + prev;
                        if (newv > 2)
                            newv = 2;
                        dp[p * cap + s] = (uint8_t)newv;
                    }
                }
            }
        }

        /* Collect sums with at least two representations */
        seq_t *terms = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!terms) abort();
        size_t tlen = 0;
        for (size_t s = 1; s <= (size_t)L; ++s) {
            if (dp[9 * cap + s] >= 2) {
                terms[tlen++] = (seq_t)s;
            }
        }

        free(dp);

        if (tlen >= n) {              /* enough terms collected */
            *out_terms = terms;
            *out_len   = tlen;
            return;
        }

        /* Not enough: free terms and double the range */
        free(terms);
        L <<= 1;                       /* double L */
    }
}

/* Public function required by the grader.  */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;

    if (n == 0)
        return 0;                       /* undefined, but guard */

    if (!terms || terms_len < n) {
        free(terms);
        compute_terms(n, &terms, &terms_len);
    }
    return terms[n - 1];                 /* 1‑based indexing */
}
