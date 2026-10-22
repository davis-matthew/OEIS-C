/*
 * A377201  Numbers k such that d(k)^k == d(k) (mod k), where d(k) =
 *          A000005(k) is the number of divisors of k.
 * https://oeis.org/A377201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 6777, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377201
 * Numbers k such that d(k)^k == d(k) (mod k),
 * where d(k) = A000005(k) is the number of divisors of k.
 * The routine lazily generates the sequence up to the requested index
 * by testing successive integers.  It keeps a dynamic table of the
 * found terms in a uint64_t array and returns the n‑th term cast to
 * seq_t.  The calculation uses divisor_count_u64 and mod_pow from
 * the util library.
 */

#include <stdlib.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/mod_pow.h"

static uint64_t *terms = NULL;   /* table of found terms */
static size_t   terms_len = 0;   /* number of stored terms   */
static size_t   terms_cap = 0;   /* allocated capacity       */
static idx_t    last_checked = 0;/* largest k that has been examined */

seq_t a(idx_t n)
{
    /* grow the table until we have at least n terms */
    while (terms_len < n)
    {
        ++last_checked;                          /* next integer to test */

        /* number of divisors of the current integer */
        uint64_t d = divisor_count_u64(last_checked);

        /* compute d^last_checked (mod last_checked)     */
        seq_t powmod = mod_pow((seq_t)d, last_checked, (seq_t)last_checked);

        /* d modulo last_checked */
        seq_t dmod = (seq_t)(d % last_checked);

        if (powmod == dmod)                      /* condition satisfied */
        {
            if (terms_len == terms_cap)          /* grow the array */
            {
                terms_cap = (terms_cap == 0) ? 32 : terms_cap * 2;
                terms = (uint64_t *)realloc(terms, terms_cap * sizeof(uint64_t));
            }
            terms[terms_len++] = last_checked;   /* store the term */
        }
    }

    /* return the requested term, converted to seq_t */
    return (seq_t)terms[n - 1];
}
