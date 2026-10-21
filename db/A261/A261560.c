/*
 * A261560  Semiprimes sp such that (sum of digits of (sp)) + (product of
 *          digits of (sp)) is also semiprime.
 * https://oeis.org/A261560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 5144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, count_prime_factors, digit_product, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Semiprimes sp such that (sum of digits of sp) + (product of digits of sp) is also semiprime.
   a(1) = 14, a(2) = 33, a(3) = 38, ...
   This implementation finds terms on demand, caching results so that subsequent calls are fast.
*/

#include <stddef.h>
#include "../utils/count_prime_factors.h"
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* seq_t holds a term, idx_t is the index type */
    if (n == 0U) return 0U;           /* guard against sub‑offset calls */

    static seq_t *terms = NULL;       /* array of found terms   */
    static size_t terms_len = 0;      /* current number of terms */
    static size_t terms_cap = 0;      /* allocated capacity      */
    static seq_t tested_before = 4U;  /* next candidate to test */

    /* return cached value if already computed */
    if ((size_t)n <= terms_len) {
        return terms[n - 1U];
    }

    /* search for more terms until we have at least n */
    for (seq_t cand = tested_before; ; ++cand) {
        if ((int)count_prime_factors(cand) == 2) {           /* semiprime?          */
            seq_t sum  = digit_sum(cand);
            seq_t prod = digit_product(cand);
            seq_t target = sum + prod;

            if ((int)count_prime_factors(target) == 2) {     /* target also semiprime */
                append_candidate(&terms, &terms_len, &terms_cap, cand);

                if ((size_t)n <= terms_len) {
                    tested_before = cand + 1U;  /* next start point */
                    return terms[n - 1U];
                }
            }
        }
    }
}
