/*
 * A385098  Numbers k such that Moebius(espf(k)) > 0, where espf(k) is the
 *          exponent of the smallest prime factor of k, with espf(1) = 1.
 * https://oeis.org/A385098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   mu_of_uint64, smallest_prime_factor_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385098: the nth positive integer k such that μ(exp_smallest_prime_factor(k)) = 1.
   We generate the terms incrementally by testing each integer k.
   For k = 1 the exponent is defined to be 1; μ(1) = 1, so 1 is the first term.
   For k > 1, we find its smallest prime factor p and the exponent e of p in k.
   If μ(e) == 1 (i.e. e is square‑free with an even number of distinct prime factors,
   or e == 1) then k is stored as a term.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mu_of_uint64.h"
#include "../utils/smallest_prime_factor_exponent.h"

/* Main sequence function requested by the build system. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;  /* stored terms a(1), a(2), … */
    static size_t cap   = 0;     /* allocated capacity of 'terms'      */
    static size_t sz    = 0;     /* number of terms currently stored   */
    static idx_t last_k = 0;     /* last integer examined              */

    /* Expand storage until we have at least n terms. */
    while (sz < (size_t)n) {
        ++last_k;                         /* next candidate k                */
        uint64_t e = smallest_prime_factor_exponent(last_k);

        /* μ(e) > 0 ⇔ μ(e) == 1 */
        if (mu_of_uint64(e) == 1) {
            if (sz == cap) {             /* grow array if necessary        */
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();       /* unrecoverable error            */
                terms = tmp;
                cap = new_cap;
            }
            terms[sz++] = (seq_t)last_k;
        }
    }
    return terms[n - 1];
}
