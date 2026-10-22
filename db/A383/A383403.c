/*
 * A383403  Partial sums of the sum of the divisors of the numbers of the form
 *          6*k + 3, k >= 0.
 * https://oeis.org/A383403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} sigma(6*k+3)
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* compute sigma using the provided utility */
#include "../utils/sigma_of.h"

static seq_t *cum_cache = NULL;   /* cumulative sums a(n) */
static seq_t *sigma_cache = NULL; /* sigma values for each 6*k+3 */
static size_t cum_cap = 0;
static size_t sigma_cap = 0;
static idx_t cum_len = 0;    /* number of a(n) values stored */
static idx_t sigma_len = 0;  /* number of sigma values stored */

/* Ensure a static array has at least 'need' elements */
static void ensure_capacity(size_t need, size_t *cap, seq_t **ptr)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 1 : *cap;
    while (new_cap < need) new_cap <<= 1; /* double until big enough */
    *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* grow cache if needed */
    if (n >= cum_len) {
        size_t new_need = (size_t)(n + 1);
        if (new_need > cum_cap) {
            ensure_capacity(new_need, &cum_cap, &cum_cache);
            ensure_capacity(new_need, &sigma_cap, &sigma_cache);
        }
        /* compute missing values */
        for (idx_t k = sigma_len; k <= n; ++k) {
            idx_t num = 6 * k + 3;            /* current number 6*k+3 */
            seq_t sig = sigma_of(num);        /* sigma(num) */
            sigma_cache[k] = sig;             /* store sigma value */

            seq_t prev = (k == 0) ? 0 : cum_cache[k - 1];
            cum_cache[k] = prev + sig;        /* cumulative sum */
        }
        sigma_len = n + 1;
        cum_len = n + 1;
    }
    return cum_cache[n];
}
