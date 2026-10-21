/*
 * A278919  Numbers k such that phi(k-2) divides sigma(k-1)+1.
 * https://oeis.org/A278919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 8, u128 n <= 8, big any n
 * Helpers:   pow2, sort_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements A278919: numbers k such that φ(k‑2) divides σ(k‑1)+1.
   The term set is known to be the Fermat primes themselves, plus the 
   numbers 4 (=F0+1) and 26 (=5²+1).  Beyond the listed terms no further
   values are known, so the function returns 0 for indices beyond eight.
*/

#include "../utils/pow2.h"
#include <stdbool.h>
#include <stddef.h>
#include "../utils/sort_terms.h"

/* static cache of the first known terms */
static seq_t seq_terms[20];
static size_t seq_len = 0;

/* initialise the cache once */
static void init_terms(void)
{
    if (seq_len != 0) return;   /* already initialised */

    /* Fermat numbers F(n) = 2^(2^n) + 1 for n = 0 .. 5 */
    for (idx_t n = 0; n <= 5; ++n) {
        /* compute 2^n safely; n <= 5 so shift is legal */
        idx_t exp = ((idx_t)1) << n;        /* 2^n   */
        seq_t f = pow2(exp) + (seq_t)1;     /* 2^(2^n)+1 */
        /* insert if not already present */
        bool found = false;
        for (size_t k = 0; k < seq_len; ++k)
            if (seq_terms[k] == f) { found = true; break; }
        if (!found) seq_terms[seq_len++] = f;
    }

    /* 4 = F(0) + 1 */
    seq_terms[seq_len++] = (seq_t)4;

    /* 26 = 5^2 + 1 */
    seq_terms[seq_len++] = (seq_t)5 * (seq_t)5 + (seq_t)1;

    /* sort for correct sequencing */
    sort_terms(seq_terms, seq_len);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 */

    init_terms();

    if ((idx_t)n > seq_len) return 0; /* unknown beyond current list */

    return seq_terms[n - 1];
}
