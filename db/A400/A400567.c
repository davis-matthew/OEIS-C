/*
 * A400567  Number of set partitions of an n-element set whose number of
 *          blocks is a Sophie Germain prime.
 * https://oeis.org/A400567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, new
 * Formula:   a(n) = ∑ S(n,p) where the sum is over primes p such that * p and
 *            2p+1 are prime (Sophie Germain primes). * This implementation
 *            builds the Stirling numbers of the second kind * row‑by‑row
 *            using the recurrence * S(n,k) = k·S(n‑1,k) + S(n‑1,k‑1), * and
 *            for each n adds the values at SG‑prime indices....
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime, mul_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Helper functions provided in the build environment */
#include "../utils/is_prime.h"           /* bool is_prime(idx_t n); */
#include "../utils/mul_small.h"          /* seq_t mul_small(seq_t x, unsigned int m); */
#include "../utils/seq_realloc.h"        /* seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap); */

seq_t a(idx_t n)
{
    /* static state for incremental computation */
    static idx_t    max_n = 0;         /* largest n computed so far */
    static size_t   cap   = 0;         /* current capacity of arrays      */
    static seq_t*   prev  = NULL;      /* previous row of Stirling numbers */
    static seq_t*   curr  = NULL;      /* current row of Stirling numbers  */
    static seq_t*   ans   = NULL;      /* cached answers a(0..max_n)       */

    /* First call: allocate capacity 1, initialise base case */
    if (prev == NULL) {
        cap = 1;
        prev = seq_realloc(prev, 0, cap);
        curr = seq_realloc(curr, 0, cap);
        ans  = seq_realloc(ans,  0, cap);
        if (cap == 0) return (seq_t)0;   /* allocation failed – safe fall‑back */
        prev[0] = (seq_t)1;   /* S(0,0)=1   */
        curr[0] = (seq_t)0;   /* not used yet */
        ans[0]  = (seq_t)0;   /* a(0)=0     */
        max_n   = 0;
    }

    /* Expand arrays if necessary  */
    if (n > max_n) {
        size_t new_cap = (size_t)n + 1;
        if (cap < new_cap) {
            prev = seq_realloc(prev, cap, new_cap);
            curr = seq_realloc(curr, cap, new_cap);
            ans  = seq_realloc(ans,  cap, new_cap);
            /* initialise newly added entries to zero */
            for (size_t i = cap; i < new_cap; ++i) {
                prev[i] = (seq_t)0;
                curr[i] = (seq_t)0;
                ans[i]  = (seq_t)0;
            }
            cap = new_cap;
            /* keep prev[0] (S(0,0)=1) unchanged */
        }

        /* Build rows from max_n+1 up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            curr[0] = (seq_t)0;    /* S(i,0)=0 for i>0 */

            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = mul_small(prev[k], (unsigned int)k); /* k·S(i‑1,k) */
                curr[k]    = term + prev[k - 1];                  /* + S(i‑1,k‑1) */
            }

            /* Sum over Sophie Germain primes up to i */
            seq_t sum = (seq_t)0;
            for (idx_t p = 1; p <= i; ++p) {
                if (is_prime(p) && is_prime(2 * p + 1)) {
                    sum += curr[p];
                }
            }
            ans[i] = sum;

            /* Prepare for next iteration: swap prev and curr */
            seq_t* tmp = prev;
            prev = curr;
            curr = tmp;
        }

        max_n = n;
    }

    return ans[n];
}
