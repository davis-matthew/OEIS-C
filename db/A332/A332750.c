/*
 * A332750  The number of flips to go from Hamiltonian cycle alpha_n to beta_n
 *          in the Cameron graph of size n using Thomason's algorithm.
 * https://oeis.org/A332750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A332750
   Recurrence: a(n) = 4*a(n-1) - a(n-2) - a(n-4) - a(n-6)  for n > 6
   Initial values: a(1)=11, a(2)=65, a(3)=265, a(4)=1005, a(5)=3749, a(6)=13927 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    // Return 0 for indices before the offset (not part of the OEIS sequence)
    if (n < 1) return 0;

    /* Cache to avoid recomputation across calls.
       cache[0] is unused – the sequence is 1‑based. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   // size of allocated array
    static idx_t computed = 0;    // largest n computed so far

    /* Ensure the array is large enough to hold a(n) */
    if (cache_len < n + 1) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;        // allocation failure – return 0
        cache = tmp;
        cache_len = n + 1;
    }

    /* Compute any missing values up to n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1)      cache[i] = (seq_t)11;
            else if (i == 2) cache[i] = (seq_t)65;
            else if (i == 3) cache[i] = (seq_t)265;
            else if (i == 4) cache[i] = (seq_t)1005;
            else if (i == 5) cache[i] = (seq_t)3749;
            else if (i == 6) cache[i] = (seq_t)13927;
            else
                cache[i] = (seq_t)4 * cache[i - 1]
                           - cache[i - 2]
                           - cache[i - 4]
                           - cache[i - 6];
        }
        computed = n;
    }

    return cache[n];
}
