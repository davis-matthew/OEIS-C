/*
 * A350059  Numbers k such that 3k and 4k have the same number of divisors.
 * https://oeis.org/A350059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A350059: numbers k such that v₂(k) – 2·v₃(k) = 1.  
   The function returns the n‑th term (n ≥ 1).  The sieve works by
   testing successive integers and recording those that satisfy the
   valuation condition.  Memoisation is used so that calls with
   arbitrary n or in any order are handled efficiently. */

#include <stdlib.h>

/* Return the n-th term of OEIS A350059 */
seq_t a(idx_t n)
{
    /* Static storage for found terms */
    static seq_t *terms      = NULL;   /* array of terms */
    static size_t terms_size = 0;      /* number of terms stored */
    static size_t terms_cap  = 0;      /* array capacity */
    static seq_t next_candidate = 1;   /* integer to test next */

    /* Ensure we have at least n terms available */
    if (terms_size < (size_t)n) {
        /* Grow the array if necessary */
        if (terms_cap < terms_size + (size_t)n - terms_size) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 64;
            while (new_cap < terms_size + (size_t)n - terms_size)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* allocation failed */
            terms = tmp;
            terms_cap = new_cap;
        }

        /* Find new terms up to the n-th one */
        for (; terms_size < (size_t)n; ++next_candidate) {
            seq_t m = next_candidate;
            int e2 = 0, e3 = 0;

            /* exponent of 2 */
            while ((m & 1) == 0) {  /* m % 2 == 0 */
                m >>= 1;
                ++e2;
            }
            /* exponent of 3 */
            while (m % 3 == 0) {
                m /= 3;
                ++e3;
            }
            if (e2 - 2 * e3 == 1) {
                terms[terms_size++] = next_candidate;
            }
        }
    }

    return terms[n - 1];
}
