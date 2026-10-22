/*
 * A359793  Dirichlet inverse of (-1)^A003415(n), where A003415 is the
 *          arithmetic derivative of n.
 * https://oeis.org/A359793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Dirichlet inverse of (-1)^{A003415(n)}. Recurrence: a(1)
 *            = 1; a(n) = -∑_{d|n, d<n} (-1)^{A003415(n/d)} * a(d) (n>1) First
 *            terms (n, a(n)): 1, 1; 2, 1; 3, 1; 4, 0; 5, 1; 6, 3; 7, 1; 8,
 *            -2; 9, 0; 10, 3; 11, 1; 12, 2; 13, 1; 14, 3; 15, 1; 16, -4; 17,
 *            1; 18, 4; 19, 1; 20, 2; 21, 1; 22, 3; 23,...
 * Range:     i8 n <= 479, i16 n <= 8436, i32 n <= 8436, i64 n <= 8436,
 *            i128 n <= 8436, big any n
 * Helpers:   _B_val, _deriv_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_B_val.h"
#include "../utils/_deriv_parity.h"

/* --------- caching for a(n) ------------------------------------------ */
static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;

/* ensure a_cache holds values up to n (inclusive) */
static void _ensure_cache(idx_t n)
{
    if (n < a_cache_size) return;
    idx_t old_size = a_cache_size;
    idx_t new_size = n + 1;            /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(a_cache, (size_t)new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);     /* allocation failure */
    a_cache = tmp;
    /* initialize new elements to 0; a_cache[1] must be 1 */
    for (idx_t i = old_size; i <= n; ++i) {
        a_cache[i] = 0;
        if (i == 1) a_cache[1] = 1;    /* base case */
    }
    a_cache_size = new_size;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of bounds, undefined */
    _ensure_cache(n);              /* make sure cache is large enough */

    /* compute any missing values in increasing order */
    for (idx_t i = 2; i <= n; ++i) {
        if (a_cache[i] != 0) continue;     /* already computed elsewhere */

        seq_t sum = 0;
        /* enumerate divisors d of i, d < i */
        for (uint64_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                uint64_t d1 = d;
                uint64_t d2 = i / d;

                if (d1 < i) {
                    seq_t b = _B_val(d2);     /* B(i/d1) */
                    sum += b * a_cache[d1];
                }
                if (d2 != d1 && d2 < i) {
                    seq_t b = _B_val(d1);     /* B(i/d2) = B(d1) */
                    sum += b * a_cache[d2];
                }
            }
        }
        a_cache[i] = -sum;
    }
    return a_cache[n];
}
