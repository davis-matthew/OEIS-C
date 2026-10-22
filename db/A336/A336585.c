/*
 * A336585  Integers m such that (m-d)*(m+d) < (m-1)^2, where d is the
 *          smallest number such that both m-d and m+d are primes.
 * https://oeis.org/A336585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: integers m such that
 *   m is composite,
 *   d = the smallest positive integer with both m−d and m+d prime,
 *   and d² > 2·m−1 (equivalently (m−d)(m+d) < (m−1)²).
 * This function returns the n‑th such integer (n≥1), memoising
 * previously computed values.  It works for any seq_t width
 * and for any order of calls.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

static seq_t *terms = NULL;     /* stored terms */
static size_t term_cap = 0;     /* capacity of the array */
static idx_t term_cnt = 0;      /* number of terms computed so far */
static idx_t next_m = 4;        /* next candidate m to test (skip 2,3) */

/* Ensure the terms array has room for at least one more entry. */
static void ensure_terms_cap(void)
{
    if (term_cnt < term_cap)
        return;
    term_cap = (term_cap == 0) ? 32 : term_cap << 1;
    seq_t *tmp = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
    if (!tmp) {
        free(terms);
        exit(EXIT_FAILURE);
    }
    terms = tmp;
}

/* Return the n‑th term (n ≥ 1).  Calls may come in any order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined, harmless return */

    /* Compute terms until we have at least n of them. */
    while (term_cnt < n) {
        idx_t m = next_m;          /* candidate m */

        /* Skip primes – for a prime m the smallest d is 0, not >0. */
        if (!is_prime_small((seq_t)m)) {
            /* Find the smallest positive d with m−d and m+d prime. */
            idx_t d;
            for (d = 1; d <= m - 2; ++d) {   /* m−d must be ≥2 */
                if (is_prime_small((seq_t)(m - d)) &&
                    is_prime_small((seq_t)(m + d)))
                    break;
            }

            if (d <= m - 2) {                 /* a valid d was found */
                /* Check d² > 2*m−1  (use 128‑bit to avoid overflow). */
                unsigned __int128 dd = (unsigned __int128)d * d;
                if (dd > (unsigned __int128)(2 * m - 1)) {
                    /* Accept m as a term. */
                    ensure_terms_cap();
                    terms[term_cnt++] = (seq_t)m;
                }
            }
        }

        ++next_m;   /* proceed to the next candidate m */
    }

    return terms[n - 1];
}
