/*
 * A393316  G.f. A(x) satisfies: A(x) = A(x^3) / (1 - x - x^2 - x^3).
 * https://oeis.org/A393316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} A000073(n-3*k+2) * a(k), a(0)=1
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* caches for a(n) and for the Tribonacci sequence A000073 */
    static seq_t *a_cache   = NULL;
    static seq_t *trib      = NULL;
    static idx_t  a_max     = 0;
    static idx_t  trib_max  = 2;
    static size_t a_cap     = 0;
    static size_t trib_cap  = 0;

    /* initialise cache for a(n) */
    if (a_cache == NULL) {
        a_cap = 16;
        a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
        if (a_cache == NULL) return (seq_t)0;           /* memory failure */
        a_cache[0] = (seq_t)1;                          /* a(0) = 1 */
        a_max = 0;
    }

    /* initialise Tribonacci table */
    if (trib == NULL) {
        trib_cap = 16;
        trib = (seq_t*)malloc(trib_cap * sizeof(seq_t));
        if (trib == NULL) return (seq_t)0;              /* memory failure */
        trib[0] = (seq_t)0;
        trib[1] = (seq_t)0;
        trib[2] = (seq_t)1;
        trib_max = 2;
    }

    /* ensure trib has values up to index n+2 */
    if (n + 2 > trib_max) {
        size_t new_cap = trib_cap;
        while (new_cap <= (size_t)(n + 2)) new_cap <<= 1;
        trib = (seq_t*)realloc(trib, new_cap * sizeof(seq_t));
        trib_cap = new_cap;
        for (idx_t i = trib_max + 1; i <= n + 2; ++i) {
            trib[i] = trib[i-1] + trib[i-2] + trib[i-3];
        }
        trib_max = n + 2;
    }

    /* compute a(n) up to requested n */
    if (n > a_max) {
        size_t new_cap = a_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap = new_cap;
        for (idx_t i = a_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t lim = i / 3;
            for (idx_t k = 0; k <= lim; ++k) {
                idx_t t = i - 3 * k + 2;      /* index in Tribonacci array */
                sum += trib[t] * a_cache[k];
            }
            a_cache[i] = sum;
        }
        a_max = n;
    }

    return a_cache[n];
}
