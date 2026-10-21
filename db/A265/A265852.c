/*
 * A265852  n such that A261807(n) = n^3 - n.
 * https://oeis.org/A265852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer m (m ≥ 0) such that * (m^3 & m) == m. *
 *            This implements OEIS sequence A265852: n such that * the base‑2
 *            representation of n^3 has a 1 wherever n has a 1 * (i.e., n &
 *            n^3 == n). The offset is 1, with a(1)=0
 * Range:     u8 n <= 33, u16 n <= 947, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h" /* ensures memory for arrays */

seq_t a(idx_t n)
{
    /* Static cache for efficiency across multiple calls. */
    static seq_t *cache = NULL;      /* array of found terms  */
    static size_t cache_len = 0;     /* number of terms stored   */
    static size_t cache_cap = 0;     /* allocated capacity       */
    static seq_t next_test = 0;      /* next integer to check   */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t m = next_test;
        /* Compute m³ and test the property. */
        seq_t cube = m * m * m;
        if ((cube & m) == m) {
            /* New term found: store it. */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = m;
        }
        /* Prepare for next candidate. */
        next_test++;
    }

    /* Return the n-th term (1‑based index). */
    return cache[n - 1];
}
