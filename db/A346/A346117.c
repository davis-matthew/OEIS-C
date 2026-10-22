/*
 * A346117  a(1) = a(2) = 1; a(n+2) = 1 + Sum_{d|n} a(d).
 * https://oeis.org/A346117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{d|n-2} a(d) for n ≥ 3; a(1) = a(2) = 1
 * Range:     u8 n <= 31, u16 n <= 159, u32 n <= 1381, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;          /* array of computed terms, index 0 unused */
static idx_t a_cache_cap = 0;          /* current allocated capacity (>= n+1) */
static idx_t a_max_computed = 0;       /* highest index already filled */

/* Ensure cache can store indices up to n (inclusive). */
static void a_ensure_cache(idx_t n)
{
    if (n + 1 <= a_cache_cap) return;
    size_t new_cap = (size_t)(n + 1);
    void *tmp = realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                       /* allocation failure → abort */
    a_cache = (seq_t *)tmp;
    a_cache_cap = (idx_t)new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;              /* undefined for n < 1 */

    a_ensure_cache(n);

    /* Initialize missing entries up to n. */
    for (idx_t k = a_max_computed + 1; k <= n; ++k) {
        if (k == 1) {
            a_cache[1] = (seq_t)1;
        } else if (k == 2) {
            a_cache[2] = (seq_t)1;
        } else {
            idx_t m = k - 2;                  /* value whose divisors sum */
            seq_t sum = (seq_t)1;             /* the leading 1 in the recurrence */
            for (idx_t d = 1; d * d <= m; ++d) {
                if (m % d == 0) {
                    sum += a_cache[d];
                    idx_t other = m / d;
                    if (other != d) sum += a_cache[other];
                }
            }
            a_cache[k] = sum;
        }
    }
    a_max_computed = (n > a_max_computed) ? n : a_max_computed;

    return a_cache[n];
}
