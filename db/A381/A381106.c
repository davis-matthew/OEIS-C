/*
 * A381106  Expansion of e.g.f. -log(1-x)^3 * (exp(x) - 1) / 6.
 * https://oeis.org/A381106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A381106 – Expansion of e.g.f. -log(1‑x)^3 * (exp(x) – 1) / 6
   Formula: a(n) = Σ_{k=0}^{n-1} binomial(n,k) * |Stirling1(k,3)|
   where |Stirling1(k,3)| is the unsigned Stirling number of the first
   kind (number of permutations of k elements with exactly 3 cycles).
   a(n) = 0 for n < 4. */

#include <stdlib.h>

/* Static tables of Stirling numbers of the first kind for k = 3.   */
static seq_t *stirling3   = NULL;      /* stirling3[k] = |S1(k,3)|          */
static size_t stirling3_cap = 0;        /* allocated capacity              */
static idx_t  last_stirling3 = 0;       /* largest k that has been computed */

/* Static cache of the sequence values a(n). */
static seq_t *seq_cache     = NULL;      /* seq_cache[n] = a(n)               */
static size_t seq_cache_cap = 0;         /* allocated capacity                */
static idx_t  last_seq      = 0;         /* largest n that has been computed */

/*======================================================================
 * Compute the Stirling numbers |S1(k,3)| for 0 ≤ k ≤ up_to.
 * The recurrence (valid for k ≥ 2):
 *   c1(k) = (k-1)!
 *   c2(k) = c1(k-1) + (k-2)*c2(k-1)
 *   c3(k) = c2(k-1) + (k-2)*c3(k-1)
 *
 * Only c3 is stored, but c1 and c2 are kept in local variables to
 * continue the recurrence across calls.
 *====================================================================*/
static void compute_stirling3_up_to(idx_t up_to)
{
    if (up_to <= last_stirling3) return;

    /* Grow the array if necessary. */
    if ((size_t)(up_to + 1) > stirling3_cap) {
        size_t new_cap = (stirling3_cap == 0) ? 16 : stirling3_cap * 2;
        while (new_cap < (size_t)(up_to + 1)) new_cap <<= 1;
        stirling3 = (seq_t *)realloc(stirling3, new_cap * sizeof(seq_t));
        stirling3_cap = new_cap;
    }

    /* Initialise for the first two indices. */
    if (last_stirling3 == 0) {
        stirling3[0] = 0;
        if (up_to >= 1) stirling3[1] = 0;
        last_stirling3 = (up_to >= 1) ? 1 : 0;
    }

    /* Variables holding previous values of the recurrence. */
    seq_t c1 = 1;          /* |S1(1,1)| = 1 */
    seq_t c2 = 0;          /* |S1(1,2)| = 0 */
    seq_t c3 = 0;          /* |S1(1,3)| = 0 */

    /* If we had already computed up to a larger index, recover the
       state from the stored values.                                       */
    if (last_stirling3 > 1) {
        /* Recompute c1,c2,c3 up to last_stirling3 to restore state. */
        c1 = 1; c2 = 0; c3 = 0;
        for (idx_t k = 2; k <= last_stirling3; ++k) {
            seq_t new_c1 = c1 * (seq_t)(k - 1);
            seq_t new_c2 = c1 + c2 * (seq_t)(k - 1);
            seq_t new_c3 = c2 + c3 * (seq_t)(k - 1);
            c1 = new_c1; c2 = new_c2; c3 = new_c3;
        }
    }

    /* Compute from last_stirling3+1 up to up_to. */
    for (idx_t k = last_stirling3 + 1; k <= up_to; ++k) {
        seq_t new_c1 = c1 * (seq_t)(k - 1);
        seq_t new_c2 = c1 + c2 * (seq_t)(k - 1);
        seq_t new_c3 = c2 + c3 * (seq_t)(k - 1);

        stirling3[k] = new_c3;   /* store |S1(k,3)| */

        c1 = new_c1; c2 = new_c2; c3 = new_c3;
    }

    last_stirling3 = up_to;
}

/*======================================================================
 * Compute the sequence values a(n) for last_seq < n ≤ up_to and
 * store them in seq_cache.
 *====================================================================*/
static void compute_seq_up_to(idx_t up_to)
{
    if (up_to <= last_seq) return;

    /* Grow the cache if necessary. */
    if ((size_t)(up_to + 1) > seq_cache_cap) {
        size_t new_cap = (seq_cache_cap == 0) ? 16 : seq_cache_cap * 2;
        while (new_cap < (size_t)(up_to + 1)) new_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache_cap = new_cap;
    }

    /* Make sure Stirling numbers are available. */
    if (up_to >= 1) compute_stirling3_up_to(up_to - 1);

    /* Compute values. */
    for (idx_t n = last_seq + 1; n <= up_to; ++n) {
        if (n < 4) {
            seq_cache[n] = 0;
            continue;
        }

        seq_t sum   = 0;
        seq_t binom = 1;      /* binomial(n,0) */

        for (idx_t k = 0; k <= n - 1; ++k) {
            if (k >= 3) {
                sum = sum + binom * stirling3[k];
            }
            if (k < n - 1) {   /* prepare next binomial */
                seq_t mult = binom * (seq_t)(n - k);
                binom       = mult / (seq_t)(k + 1);
            }
        }
        seq_cache[n] = sum;
    }

    last_seq = up_to;
}

/*======================================================================
 * Public function to obtain the nth element of the sequence.
 *====================================================================*/
seq_t a(idx_t n)
{
    /* First‑time initialisation of the cache. */
    if (seq_cache == NULL) {
        size_t init_cap = 16;
        seq_cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        seq_cache_cap = init_cap;
        seq_cache[0] = 0;
        last_seq = 0;
    }

    if (n >= seq_cache_cap || n > last_seq) {
        compute_seq_up_to(n);
    }
    return seq_cache[n];
}
