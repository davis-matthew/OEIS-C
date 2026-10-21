/*
 * A244014  Numerators of rational approximations to sqrt(6) obtained from
 *          Newton's method.
 * https://oeis.org/A244014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the numerators of Newton's approximations to sqrt(6)
   (see OEIS A244014):
   Let p0 = 2, q0 = 1.
   For n ≥ 0:
       p_{n+1} = p_n^2 + 6 q_n^2
       q_{n+1} = 2 p_n q_n
       g      = gcd(p_{n+1}, q_{n+1})
       p_{n+1} = p_{n+1} / g
       q_{n+1} = q_{n+1} / g
   Then a(n) = p_n (numerator after reduction). */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *num_arr  = NULL;
static seq_t *den_arr  = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (cache_len == 0) {
        cache_cap = 16;  /* initial capacity */
        num_arr = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        den_arr = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!num_arr || !den_arr) abort();
        num_arr[0] = (seq_t)2;
        den_arr[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Return cached value if already computed */
    if ((size_t)n < cache_len)
        return num_arr[(size_t)n];

    /* Compute terms up to requested index */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t p_prev = num_arr[i - 1];
        seq_t q_prev = den_arr[i - 1];

        /* p_new = p_prev^2 + 6 * q_prev^2 */
        seq_t p_new = p_prev * p_prev;
        seq_t q_sq  = q_prev * q_prev;
        p_new += (seq_t)6 * q_sq;

        /* q_new = 2 * p_prev * q_prev */
        seq_t q_new = (p_prev * q_prev) * ((seq_t)2);

        /* Reduce by gcd */
        seq_t g = gcd(p_new, q_new);
        p_new /= g;
        q_new /= g;

        /* Resize cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= i + 1) new_cap = i + 1;
            seq_t *tmp = (seq_t *)realloc(num_arr, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            num_arr = tmp;
            tmp = (seq_t *)realloc(den_arr, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            den_arr = tmp;
            cache_cap = new_cap;
        }

        num_arr[i] = p_new;
        den_arr[i] = q_new;
        ++cache_len;
    }

    return num_arr[(size_t)n];
}
