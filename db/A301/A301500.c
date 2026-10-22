/*
 * A301500  Number of compositions (ordered partitions) of n into squarefree
 *          parts (A005117) such that no two adjacent parts are equal (Carlitz
 *          compositions).
 * https://oeis.org/A301500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient sequence of 1/(1 - Sum_{k>=1} mu(k)^2 *
 *            x^k/(1 + x^k)), i.e. a(n) = Sum_{k=1..n} c(k) * a(n-k) with
 *            a(0)=1 where c(k) = sum_{d|k, d squarefree} (-1)^{k/d-1}
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   compute_coefficients
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   ----  Sequence a(n) for squarefree compositions with no equal neighbours  ----   */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/compute_coefficients.h"

/*  static state for memoisation  */
static seq_t *a_cache = NULL;          /* a_cache[0..max]  */
static idx_t a_len = 0;                /* number of a_cache values currently stored   */

static int *c_cache = NULL;            /* c_cache[0..max]  */
static idx_t c_len = 0;                /* number of c_cache values currently stored   */

/* -------------------------------------------------------------------------- */
/*  Compute the coefficient array c[1..N]   */
/*  c[k] = sum_{d|k, d squarefree} (-1)^{k/d-1}                                 */

/* -------------------------------------------------------------------------- */
/*  Main sequence function                                                  */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /*  Ensure coefficient array covers at least n  */
    if (c_len <= n) {
        int *new_c = (int*)malloc((size_t)(n + 1) * sizeof(int));
        compute_coefficients(n, new_c);
        free(c_cache);
        c_cache = new_c;
        c_len   = n + 1;
    }

    /*  Initialise a_cache if empty  */
    if (a_len == 0) {
        a_len   = 1;
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        a_cache[0] = 1;          /* a(0) = 1 */
    }

    /*  Extend a_cache up to n  */
    if (a_len <= n + 1) {
        seq_t *new_a = (seq_t*)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
        a_cache = new_a;
        for (idx_t i = a_len; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                int coeff = c_cache[k];
                if (coeff == 0) continue;
                seq_t term = a_cache[i - k] * (seq_t)coeff;
                sum += term;
            }
            a_cache[i] = sum;
        }
        a_len = n + 1;
    }

    return a_cache[n];
}
