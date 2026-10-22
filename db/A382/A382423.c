/*
 * A382423  The number of exponents in the prime factorization of n-th
 *          biquadratefree number that are equal to 2.
 * https://oeis.org/A382423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of prime exponents equal to 2 in the
 *            factorisation of the n‑th biquadrate‑free integer (a
 *            biquadrate‑free integer is one not divisible by any 4th power).
 *            The code generates integers on demand, tests them using a
 *            smallest‑prime‑factor sieve, stores the counts in a static
 *            cache,...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

/* Return the number of prime exponents equal to 2 in the factorisation
   of the n‑th biquadrate‑free integer.  */
seq_t a(idx_t n)
{
    /* ----- static state for caching ----- */
    static seq_t *cache          = NULL;   /* computed results       */
    static size_t cache_size    = 0;       /* number of cached entries */
    static size_t cache_cap     = 0;       /* allocated capacity      */
    static idx_t checked_num    = 1;       /* next integer to examine */
    static idx_t *spf           = NULL;   /* smallest‑prime‑factor array */
    static idx_t  spf_limit     = 0;       /* current spf array length  */

    /* ----- extend cache until it contains at least n entries ----- */
    while (cache_size < (size_t)n) {
        /* Ensure SPF array covers 'checked_num' */
        while (checked_num > spf_limit) {
            idx_t new_limit = (spf_limit == 0) ? 32 : spf_limit * 2;
            if (new_limit < checked_num)
                new_limit = checked_num;
            if (spf != NULL)
                free(spf);
            build_spf(new_limit, &spf);      /* allocates array of size new_limit+1 */
            spf_limit = new_limit;
        }

        /* ----- factor 'checked_num' using SPF ----- */
        idx_t  m           = checked_num;
        bool   bad         = false;
        seq_t  two_exp_cnt = 0;

        while (m > 1) {
            idx_t p = spf[m];
            if (p == 0)    /* safety: treat 0 as the number itself */
                p = m;
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
                if (exp >= 4) {    /* divisible by a 4th power → not biquadrate‑free */
                    bad = true;
                    break;
                }
            }
            if (bad)
                break;
            if (exp == 2)
                two_exp_cnt++;
        }

        /* ----- store result if biquadrate‑free ----- */
        if (!bad) {
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_size++] = two_exp_cnt;
        }

        checked_num++;
    }

    /* ----- return the cached value for index n (1‑based) ----- */
    return cache[n - 1];
}
