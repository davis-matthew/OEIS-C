/*
 * A251575  E.g.f.: exp(5*x*G(x)^4) / G(x)^4 where G(x) = 1 + x*G(x)^5 is the
 *          g.f. of A002294.
 * https://oeis.org/A251575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation follows the convolution formula derived from the
 * explicit exponential generating function of A251575:
 *
 *   a(0) = 1
 *   a(n) = Σ_{k=1..n} a(n-k) *
 *          binomial(5k-2, k)/(4k-1) * (n-1)!/(n-k)!      (n ≥ 1)
 *
 * The auxiliary coefficient
 *   c(k) = binomial(5k-2, k)/(4k-1)
 * is pre‑computed on demand.  The factorial ratio
 *   (n-1)!/(n-k)! = Π_{t=1..k-1} (n-t)
 * is accumulated incrementally during the convolution.
 *
 * The code caches results in static arrays so repeated calls are
 * O(1) after the needed values have been computed.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <string.h>   /* memset */

#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) -> seq_t */

static seq_t *a_arr   = NULL;   /* a[n] , n ≥ 0   */
static size_t a_cap   = 0;
static idx_t  a_max   = 0;       /* maximum n computed in a_arr */

static seq_t *c_arr   = NULL;   /* c[k] , k ≥ 1   */
static size_t c_cap   = 0;
static idx_t  c_max   = 0;       /* maximum k computed in c_arr */

/* Ensure that c_arr has entries up to index n (inclusive).  */
static void ensure_c(idx_t n)
{
    if (c_arr == NULL) {
        /* Allocate space for indices 0..n (index 0 unused).  */
        c_cap = n + 1;
        c_arr = (seq_t *)malloc(c_cap * sizeof(seq_t));
        if (c_arr == NULL) exit(EXIT_FAILURE);
        memset(c_arr, 0, c_cap * sizeof(seq_t));
        c_max = 0;              /* no valid entries yet */
    } else if ((size_t)(n + 1) > c_cap) {
        /* Grow the array.  */
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(c_arr, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        c_arr = tmp;
        memset(c_arr + c_cap, 0, (new_cap - c_cap) * sizeof(seq_t));
        c_cap = new_cap;
    }

    /* Compute missing c[k] values.  */
    for (idx_t k = c_max + 1; k <= n; ++k) {
        seq_t num = binom(5 * k - 2, k);
        seq_t den = (seq_t)(4 * k - 1);           /* division is exact */
        c_arr[k] = num / den;
    }
    if (n > c_max) c_max = n;
}

/* Ensure that a_arr has entries up to index n (inclusive).  */
static void ensure_a(idx_t n)
{
    if (a_arr == NULL) {
        /* Start with a[0] = 1.  */
        a_cap = n + 1;
        a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (a_arr == NULL) exit(EXIT_FAILURE);
        memset(a_arr, 0, a_cap * sizeof(seq_t));
        a_arr[0] = (seq_t)1;
        a_max = 0;
    } else if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        a_arr = tmp;
        memset(a_arr + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
        a_cap = new_cap;
    }

    /* Compute a values from a_max+1 up to n.  */
    for (idx_t cur = a_max + 1; cur <= n; ++cur) {
        ensure_c(cur);                 /* c[1..cur] are ready */

        seq_t sum  = 0;
        seq_t prod = 1;                /* initially (cur-1)!/(cur-1)! = 1 */

        for (idx_t k = 1; k <= cur; ++k) {
            if (k > 1) {
                /* Extend product: multiply by (cur - k + 1).  */
                prod *= (seq_t)(cur - k + 1);
            }
            seq_t term = a_arr[cur - k] * c_arr[k];
            term *= prod;
            sum += term;
        }
        a_arr[cur] = sum;
    }
    a_max = n;
}

seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;          /* defensive, idx_t is unsigned */

    ensure_a(n);
    return a_arr[n];
}
