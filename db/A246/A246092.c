/*
 * A246092  Paradigm shift sequence for (3,2) production scheme with
 *          replacement.
 * https://oeis.org/A246092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coeff[n-1] + 4*a(n-11) for n > 11, * and a(n) =
 *            coeff[n-1] for 1 ≤ n ≤ 11, * where coeff is the list of
 *            coefficients of the numerator polynomial * of the generating
 *            function of A246092 (see the OEIS entry)
 * Range:     u8 n <= 36, u16 n <= 80, u32 n <= 168, u64 n <= 344,
 *            u128 n <= 696, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t coeff[35] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 8, 5, 3, 2, 1,
     0, 0, 0, 0, 0, 1, 2, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5
};

static seq_t *cache   = NULL;   /* array of previously computed a(i) */
static size_t cache_cap = 0;    /* allocated capacity of cache   */
static idx_t  cached_max = 0;   /* highest n that has been computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but safe for underflow */
    /* enlarge cache if necessary */
    if (n > cached_max) {
        size_t new_cap = (size_t)n;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);      /* allocation failed */
            cache     = tmp;
            cache_cap = new_cap;
        }
        /* compute terms from cached_max+1 up to n */
        for (idx_t i = cached_max; i < n; ++i) {   /* i is 0-based index */
            idx_t idx = i + 1;   /* term number (1‑based) */
            if (idx <= 11) {
                cache[i] = coeff[idx - 1];
            } else {
                seq_t prev = cache[i - 11];
                seq_t add  =
                    (idx - 1 < (idx_t)(sizeof(coeff)/sizeof(coeff[0])))
                        ? coeff[idx - 1] : 0;
                cache[i] = add + prev * 4;
            }
        }
        cached_max = n;
    }
    return cache[n - 1];
}
