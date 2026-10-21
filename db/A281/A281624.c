/*
 * A281624  Numbers m such that 2^phi(m) + 1 is prime (Fermat prime).
 * https://oeis.org/A281624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   is_prime_big, phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281624: numbers m such that 2^phi(m)+1 is a Fermat prime. */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/phi.h"       /* seq_t phi(idx_t m) */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n) */
#include "../utils/pow2.h"      /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* Cache of found terms, stored in ascending order. */
    static seq_t *terms = NULL;
    static size_t terms_size = 0;   /* number of terms stored */
    static size_t terms_cap  = 0;   /* allocated capacity */
    static idx_t m = 1;             /* next integer to test */

    /* Ensure the cache has room for a new term. */
    if (terms_size + 1 > terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure, give up */
        terms = tmp;
        terms_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (terms_size < n) {
        seq_t phiv = phi(m);           /* φ(m) */
        if ((phiv & (phiv - 1)) == 0) { /* φ(m) must be a power of two */
            seq_t val = pow2((idx_t)phiv);/* 2^φ(m) */
            val += (seq_t)1;                 /* 2^φ(m)+1 */
            if (is_prime_big(val)) {         /* prime test */
                terms[terms_size++] = (seq_t)m;
            }
        }
        ++m;
    }

    return terms[n - 1];
}
