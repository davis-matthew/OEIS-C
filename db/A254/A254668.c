/*
 * A254668  Number of ways to write n as the sum of a square, a second
 *          pentagonal number, and a hexagonal number.
 * https://oeis.org/A254668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(i,j,k) ∈ ℕ₀³ : i² + j(3j+1)/2 + k(2k−1) = n}| This
 *            counts the representations of n as a square + a pentagonal
 *            number + a hexagonal number
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9610 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Free a previously allocated counts array. */
static void free_counts(seq_t *ptr)
{
#ifdef __cplusplus
    delete [] ptr;
#else
    free(ptr);
#endif
}

/* Allocate a new array of seq_t of given size.
   New elements are zero‑initialised. */
static seq_t *new_counts(idx_t size)
{
    seq_t *ptr;
#ifdef __cplusplus
    ptr = new seq_t[size];
    for (idx_t i = 0; i < size; ++i)
        ptr[i] = 0;
#else
    ptr = (seq_t *)calloc((size_t)size, sizeof(seq_t));
#endif
    return ptr;
}

static seq_t *counts = NULL;       /* memoised counts array        */
static idx_t  max_n   = (idx_t)-1; /* last index computed           */

seq_t a(idx_t n)
{
    /* Re‑compute the table if we have no table yet, or if the
       requested index is larger than any previously computed. */
    if (counts == NULL || n > max_n)
    {
        /* Allocate (or reallocate) a table up to index n.  All elements
           are zero‑initialised. */
        seq_t *newtbl = new_counts(n + 1);
        if (!newtbl) return 0;         /* allocation failed */

        /* If we already had a table, free it. */
        if (counts) free_counts(counts);

        counts = newtbl;

        /* Enumerate all triples (i,j,k) with i² + Pi + Hk ≤ n, where
           Pi = j(3j+1)/2 and Hk = k(2k−1) (hexagonal number, with
           H0 = 0).  For each triple we increment the corresponding
           table entry. */
        for (idx_t i = 0;; ++i)
        {
            idx_t sq = i * i;
            if (sq > n) break;

            for (idx_t j = 0;; ++j)
            {
                idx_t pent = j * ((3 * j) + 1) / 2;
                idx_t s1 = sq + pent;
                if (s1 > n) break;

                for (idx_t k = 0;; ++k)
                {
                    /* hexagonal number: 0 for k=0, otherwise k(2k−1) */
                    idx_t hex = (k == 0) ? 0 : k * ((2 * k) - 1);
                    idx_t sum = s1 + hex;
                    if (sum > n) break;

                    ++counts[sum];
                }
            }
        }
        max_n = n;
    }

    /* Return the memoised value. */
    return counts[n];
}
