/*
 * A317882  Number of free pure achiral multifunctions (with empty expressions
 *          allowed) with one atom and n positions.
 * https://oeis.org/A317882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Sum_{k=1}^{n-2} a(k) * Sum_{d|(n-k-1)} a(d),
 *            a(1)=1
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 44, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static memoisation tables (1‑based indices for a(n)) */
static seq_t *A = NULL;   /* a(n) values */
static seq_t *S = NULL;   /* divisor‑sum array: S[m] = Σ_{d|m} a(d) */
static idx_t cap     = 0; /* allocated table size (≥ current n+1) */
static idx_t computed = 0;/* highest n already computed   */

static void recompute_divisor_sums(void)
{
    /* S is already zero‑filled by the caller */
    for (idx_t d = 1; d <= computed; ++d) {
        seq_t ad = A[d];
        for (idx_t m = d; m < cap; m += d)
            S[m] += ad;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* offset is 1 so below that is undefined */

    /* Grow tables if necessary */
    if (cap <= n) {
        idx_t new_cap = (cap) ? cap << 1U : 8U;
        while (new_cap <= n) new_cap <<= 1U;

        seq_t *Anew = (seq_t*)malloc(new_cap * sizeof(seq_t));
        seq_t *Snew = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!Anew || !Snew) exit(1);          /* allocation failed */

        /* Zero the new arrays first */
        memset(Anew, 0, new_cap * sizeof(seq_t));
        memset(Snew, 0, new_cap * sizeof(seq_t));

        /* Copy old data and free the old buffers */
        if (A) {
            memcpy(Anew, A, cap * sizeof(seq_t));
            free(A);
        }
        if (S) free(S);   /* old S will be recomputed from scratch */

        /* Adopt the new buffers */
        A = Anew;
        S = Snew;
        cap = new_cap;

        /* Recompute divisor sums for all indices < cap using the already
           computed A values (indices 1..computed). */
        recompute_divisor_sums();
    }

    /* If we already have the value, just return it */
    if (computed >= n) return A[n];

    /* Compute new terms up to n */
    for (idx_t idx = computed + 1; idx <= n; ++idx) {
        if (idx == 1) {
            A[1] = 1;                 /* base case */
        } else {
            seq_t sum = A[idx-1];
            if (idx > 2) {              /* inner double sum */
                for (idx_t k = 1; k <= idx-2; ++k) {
                    sum += A[k] * S[idx - k - 1];
                }
            }
            A[idx] = sum;
        }

        /* Update divisor-sum table: add a(idx) to all multiples of idx */
        for (idx_t m = idx; m < cap; m += idx)
            S[m] += A[idx];
    }

    computed = n;
    return A[n];
}
