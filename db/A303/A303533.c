/*
 * A303533  Number of ordered direct sum decompositions of the vector space
 *          GF(2)^n.
 * https://oeis.org/A303533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} [n choose k]_2 * a(n-k) * 2^{k*(n-k)}
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where [n choose k]_2 is the Gaussian binomial coefficient over GF(2). */

#include <stdlib.h>
#include <stddef.h>

/* Ensure that a realloc result is correctly typed for both C and C++. */
static void enlarge(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap)
        return;
    size_t newcap = (*cap == 0) ? 1 : (*cap << 1);
    while (newcap < needed)
        newcap <<= 1;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
       Static state for memoisation
       ------------------------------------------------------------------ */
    static seq_t *cache_a = NULL;  /* a(0) … a(current_max) */
    static size_t cap_a  = 0;
    static size_t len_a  = 0;

    /* ------------------------------------------------------------------
       Buffers for Gaussian binomial rows: prev_row corresponds to n-1,
       cur_row to current n.  They hold indices 0 … current_n.
       ------------------------------------------------------------------ */
    static seq_t *prev_row = NULL;
    static size_t cap_prev = 0;
    static size_t len_prev = 0;

    static seq_t *cur_row  = NULL;
    static size_t cap_cur  = 0;

    /* ------------------------------------------------------------------
       Initialise the state on first call
       ------------------------------------------------------------------ */
    if (!cache_a) {
        enlarge(&cache_a, &cap_a, 1);
        cache_a[0] = 1;          /* a(0) = 1 */
        len_a = 1;

        enlarge(&prev_row, &cap_prev, 1);
        prev_row[0] = 1;          /* [0 choose 0]_2 = 1 */
        len_prev = 1;
    }

    /* ------------------------------------------------------------------
       Compute terms up to the requested n (if necessary)
       ------------------------------------------------------------------ */
    while (len_a <= (size_t)n) {
        idx_t m = (idx_t)len_a;      /* compute a(m) where 1 ≤ m ≤ n */

        /* Prepare cur_row of length m+1 and compute Gaussian binomial row */
        enlarge(&cur_row, &cap_cur, (size_t)m + 1);
        cur_row[0] = 1;                /* [m choose 0]_2 = 1 */

        for (idx_t k = 1; k < m; ++k) {
            seq_t left  = prev_row[k];                    /* [m-1 choose k]     */
            seq_t right = prev_row[k - 1];                /* [m-1 choose k-1]   */
            right <<= (size_t)(m - k);                    /* ×2^{m-k}           */
            cur_row[k] = left + right;                    /* recurrence        */
        }
        cur_row[m] = 1;            /* [m choose m]_2 = 1 */

        /* Compute a(m) = Σ_{k=1..m} cur_row[k] * a(m-k) * 2^{k*(m-k)} */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            idx_t exp   = k * (m - k);               /* exponent for 2^{k(m-k)} */
            seq_t term  = cur_row[k];
            term *= cache_a[(size_t)(m - k)];        /* ×a(m-k)             */
            term <<= (unsigned long long)exp;        /* ×2^{k(m-k)}         */
            sum += term;
        }

        enlarge(&cache_a, &cap_a, len_a + 1);
        cache_a[len_a] = sum;
        ++len_a;                                /* now cache_a holds up to a(m) */

        /* Prepare for the next iteration: swap prev_row and cur_row */
        seq_t *tmp       = prev_row;
        prev_row         = cur_row;
        cur_row          = tmp;
        size_t tmp_cap   = cap_prev;
        cap_prev         = cap_cur;
        cap_cur          = tmp_cap;
        len_prev         = (size_t)m + 1;        /* new prev_row length */
    }

    return cache_a[(size_t)n];
}
