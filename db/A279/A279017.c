/*
 * A279017  a(n) = unreduced numerator in Sum_{k=1..n}(1/k^k).
 * https://oeis.org/A279017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; for n>0, a(n)=n^n*a(n-1)+∏_{k=1}^{n-1}k^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

static seq_t *a_cache = NULL;      /* a values, cached up to (cached-1) */
static seq_t *p_cache = NULL;      /* product ∏_{k=1}^{i}k^k for i cached-1 */
static size_t cached = 0;          /* number of entries currently cached */

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 0) return (seq_t)0;

    /* initialize caches if needed */
    if (cached == 0) {
        size_t init_size = 1;       /* store a(0) and p(0) */
        a_cache = mem_new(seq_t, init_size);
        p_cache = mem_new(seq_t, init_size);
        if (!a_cache || !p_cache) return (seq_t)0;   /* allocation failure */
        a_cache[0] = (seq_t)0;     /* a(0) = 0 */
        p_cache[0] = (seq_t)1;     /* product up to 0 is 1 */
        cached = 1;
    }

    /* extend caches up to n */
    if (cached <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        a_cache = mem_resize(seq_t, a_cache, new_size);
        p_cache = mem_resize(seq_t, p_cache, new_size);
        if (!a_cache || !p_cache) return (seq_t)0;   /* allocation failure */

        /* compute missing values */
        for (size_t k = cached; k <= (size_t)n; ++k) {
            seq_t kk      = (seq_t)k;
            seq_t pow_kk  = ipow(kk, kk);          /* k^k */
            a_cache[k] = pow_kk * a_cache[k - 1] + p_cache[k - 1];
            p_cache[k] = p_cache[k - 1] * pow_kk;
        }
        cached = new_size;
    }

    return a_cache[n];
}
