/*
 * A243513  Number of length n+2 0..4 arrays with no three elements in a row
 *          with pattern aba or abb (with a!=b) and new values 0..4 introduced
 *          in 0..4 order.
 * https://oeis.org/A243513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243513: Number of length n+2 0..4 arrays with no
   three-element pattern aba or abb (a≠b), with new values introduced
   in 0..4 order.
   Closed form:  a(n) = (7 + 3^n + 2n) / 4   (valid for n ≥ 1)
   Initial values: a(1)=3, a(2)=5, a(3)=10
   Recurrence for n≥4: a(n) = 5*a(n-1) - 7*a(n-2) + 3*a(n-3)   */

/* ------------------------------------------------------------------ */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Safe‑guard for unsupported indices */
    if (n == 0) return (seq_t)0;  /* OEIS offset = 1 */

    /* Memoised sequence table */
    static seq_t *cache = NULL;
    static idx_t computed = 0;    /* Highest index that is already stored */

    /* Grow the table if needed */
    if (n > computed) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)n * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;  /* allocation failure */
        cache = new_cache;

        /* Compute new entries (not recomputing any older entry) */
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)3;                /* a(1) */
            } else if (i == 2) {
                cache[1] = (seq_t)5;                /* a(2) */
            } else if (i == 3) {
                cache[2] = (seq_t)10;               /* a(3) */
            } else {
                /* a(i) = 5*a(i-1) - 7*a(i-2) + 3*a(i-3)   */
                seq_t val = (seq_t)5 * cache[(size_t)(i-2)];
                val -= (seq_t)7 * cache[(size_t)(i-3)];
                val += (seq_t)3 * cache[(size_t)(i-4)];
                cache[(size_t)(i-1)] = val;
            }
        }
        computed = n;
    }

    return cache[(size_t)(n-1)];
}
