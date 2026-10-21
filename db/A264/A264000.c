/*
 * A264000  Number of (n+1) X (3+1) arrays of permutations of 0..n*4+3 with
 *          each element having index change (+-,+-) 0,0 1,2 or 1,0.
 * https://oeis.org/A264000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29*a(n-1) + 36*a(n-2) - 2832*a(n-3) + 6656*a(n-4) +
 *            24576*a(n-5) - 65536*a(n-6) a(1) = 81, a(2) = 1089, a(3) =
 *            34225, a(4) = 833569, a(5) = 22819729, a(6) = 604028929 (those
 *            six initial values are required to start the recurrence)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Zero‑indexed array that holds values from a(0) up to a(current_max)   */
    /* a(0) is unused but kept for ease of indexing to 1‑based values       */
    static seq_t *memo = NULL;
    static idx_t  memo_cap = 0;   /* number of allocated slots   */

    /* Ensure we have at least 7 slots (indices 0‑6) to store the initial 6 terms */
    if (memo == NULL || memo_cap < 7) {
        size_t new_cap = (memo_cap < 7) ? 7 : memo_cap;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE); /* allocation failure */
        memo = tmp;
        if (memo_cap < 7) {
            /* zero‑initialize any newly allocated entries */
            memset(memo + memo_cap, 0, (new_cap - memo_cap) * sizeof(seq_t));
        }
        memo_cap = new_cap;

        /* Store the six explicit initial values (a(0) unused).          */
        if (memo[1] == 0) memo[1] = (seq_t)81u;
        if (memo[2] == 0) memo[2] = (seq_t)1089u;
        if (memo[3] == 0) memo[3] = (seq_t)34225u;
        if (memo[4] == 0) memo[4] = (seq_t)833569u;
        if (memo[5] == 0) memo[5] = (seq_t)22819729u;
        if (memo[6] == 0) memo[6] = (seq_t)604028929u;
    }

    /* If n is within the already computed range, simply return it. */
    if (n < memo_cap && memo[n] != 0) {
        return memo[n];
    }

    /* Expand memo to accommodate up to index n.  Use a growth factor to keep
       amortised cost low.  The ratio 2 is more than enough for the expected
       n (up to about 10000). */
    if (n >= memo_cap) {
        idx_t new_cap = memo_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* Zero any newly added slots */
        memset(tmp + memo_cap, 0, (new_cap - memo_cap) * sizeof(seq_t));
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Compute missing values up to n using the recurrence.  The loop
       starts at the first index that is not yet known, i.e. the
       smallest k for which memo[k] == 0 and k >= 7.  Since the first
       six values are defined, this starts at k = 7. */
    idx_t k;
    for (k = 7; k <= n; ++k) {
        seq_t v =   memo[k-1] * (seq_t)29u
                  + memo[k-2] * (seq_t)36u
                  - memo[k-3] * (seq_t)2832u
                  + memo[k-4] * (seq_t)6656u
                  + memo[k-5] * (seq_t)24576u
                  - memo[k-6] * (seq_t)65536u;
        memo[k] = v;
    }

    return memo[n];
}
