/*
 * A368680  Sum of the numbers k less than n and not dividing n such that n-k
 *          is squarefree.
 * https://oeis.org/A368680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{n-1} k · 1_{k∤n} · is_squarefree(n‑k)
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A368680: Sum of k < n, k ∤ n, with n-k squarefree */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Base case: a(1) = 0, no k < 1 */
    if (n < 2) return (seq_t)0;

    /* Cache of square‑free flags up to the largest n seen so far.
       The array is 1‑based, index 0 is unused. */
    static bool *sq = NULL;
    static idx_t max_n = 0;

    if (n > max_n) {                        /* need to enlarge the cache */
        /* Allocate (n+1) booleans and cast the void* for C++ compatibility */
        bool *tmp = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
        if (!tmp) return (seq_t)0;          /* allocation failure – abort */

        /* Initialise all entries as square‑free */
        for (idx_t i = 0; i <= n; ++i) tmp[i] = true;
        tmp[0] = false;                     /* unused */

        /* Mark multiples of perfect squares as not square‑free */
        for (idx_t p = 2; p * p <= n; ++p) {
            idx_t sqr = p * p;
            for (idx_t j = sqr; j <= n; j += sqr) {
                tmp[j] = false;
            }
        }

        /* Replace the old cache (freeing only if it existed). */
        if (sq) free(sq);
        sq     = tmp;
        max_n  = n;
    }

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k < n; ++k) {
        if (n % k == 0) continue;           /* skip divisors of n   */
        if (!sq[n - k]) continue;           /* skip if n-k not square‑free */
        sum += (seq_t)k;
    }
    return sum;
}
