/*
 * A349790  a(1) = 1; a(n) = -Sum_{d|n, d < n} lpf(n/d) * a(d).
 * https://oeis.org/A349790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = -Sum_{d|n, d < n} lpf(n/d) * a(d)
 * Range:     i8 n <= 29, i16 n <= 839, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: lpf() returns the least prime factor of its argument.   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/lpf.h"

static seq_t *a_cache      = NULL;   /* table of already computed values */
static bool  *a_computed   = NULL;   /* flag whether value is valid     */
static idx_t  a_cap        = 0;       /* current capacity of a_cache     */
static idx_t  last_computed = 0;     /* largest index that has been      */

/* Ensure the cache can hold entries up to index n.                 */
static void ensure_cache(idx_t n)
{
    if (a_cache == NULL) {
        /* initialise for indices 0 and 1 (index 0 unused) */
        a_cap = 2;
        a_cache    = (seq_t*)calloc(a_cap, sizeof(seq_t));
        a_computed = (bool*)  calloc(a_cap, sizeof(bool));
        if (!a_cache || !a_computed) exit(1);
        a_cache[1]      = (seq_t)1;     /* a(1) = 1 */
        a_computed[1]   = true;
        last_computed   = 1;
        return;
    }
    if (a_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp_cache    = (seq_t*)realloc(a_cache,    new_cap * sizeof(seq_t));
        bool  *tmp_computed = (bool*) realloc(a_computed, new_cap * sizeof(bool));
        if (!tmp_cache || !tmp_computed) exit(1);
        /* zero-initialize the newly allocated part */
        memset(tmp_cache + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
        memset(tmp_computed + a_cap, 0, (new_cap - a_cap) * sizeof(bool));
        a_cache    = tmp_cache;
        a_computed = tmp_computed;
        a_cap      = new_cap;
    }
}

/* Return the nth term of sequence A349790.                          */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, but return 0 */
    ensure_cache(n);

    if (a_computed[n]) return a_cache[n];

    /* Compute missing terms in increasing order.                  */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d < m; ++d) {
            if (m % d == 0) {
                idx_t q = m / d;
                seq_t l = (seq_t)lpf(q);   /* least prime factor of q */
                sum += l * a_cache[d];
            }
        }
        a_cache[m] = -sum;
        a_computed[m] = true;
    }
    last_computed = n;
    return a_cache[n];
}
