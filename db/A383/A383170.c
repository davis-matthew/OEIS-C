/*
 * A383170  Expansion of e.g.f. -log(1 + log(1 - 2*x)/2).
 * https://oeis.org/A383170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^(n-k) * |Stirling1(n,k) * Stirling1(k,1)|
 *            = Sum_{k=1..n} 2^(n-k) * c(n,k) * (k-1)! where c(n,k) are
 *            unsigned Stirling numbers of first kind. We compute the sequence
 *            via the recurrence for h_k(n) = c(n,k) * (k-1)! * 2^(n-k) :
 *            h_1(1) = 1 h_k(n) = (k-1)*h_{k-1}(n-1) + 2*(n-1)*h...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Computes terms of OEIS sequence A383170 */
seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *a_cache = NULL;
    static size_t a_cap = 0;
    static seq_t *h_cache = NULL;      /* holds h_k for current n */
    static size_t h_cap = 0;
    static idx_t max_n = 0;            /* largest n already computed */

    /* Initialisation for n = 0 */
    if (a_cache == NULL) {
        a_cache = (seq_t*)calloc(1, sizeof(seq_t));   /* a_cache[0] = 0 */
        a_cap   = 1;
    }
    if (n < a_cap) return a_cache[n];

    /* Initialise h_cache if not yet done */
    if (h_cache == NULL) {
        h_cache = (seq_t*)calloc(1, sizeof(seq_t));   /* h_cache[0] = 0 */
        h_cap   = 1;
    }

    /* Grow caches and compute new terms up to n */
    while (max_n < n) {
        idx_t next = max_n + 1;                     /* new n to compute */

        /* Ensure h_cache has room for indices up to next */
        if (h_cap <= (size_t)next) {
            size_t new_cap = (size_t)(next + 1);
            h_cache = (seq_t*)realloc(h_cache, new_cap * sizeof(seq_t));
            /* Initialise the new memory to zero */
            memset(h_cache + h_cap, 0,
                   (new_cap - h_cap) * sizeof(seq_t));
            h_cap = new_cap;
        }

        /* Ensure a_cache has room for index next */
        if (a_cap <= (size_t)next) {
            size_t new_cap = (size_t)(next + 1);
            a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
            memset(a_cache + a_cap, 0,
                   (new_cap - a_cap) * sizeof(seq_t));
            a_cap = new_cap;
        }

        if (next == 1) {               /* base case */
            h_cache[1] = (seq_t)1;
            a_cache[1] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            /* Update h_k in descending order to use old values */
            for (idx_t k = next; k; --k) {
                seq_t old_h_k     = h_cache[k];
                seq_t old_h_km1   = h_cache[k-1];
                seq_t term1 = (seq_t)(k-1) * old_h_km1;
                seq_t term2 = (seq_t)(2*(next-1)) * old_h_k;
                h_cache[k] = term1 + term2;
                sum += h_cache[k];
            }
            a_cache[next] = sum;
        }

        max_n = next;
    }

    return a_cache[n];
}
