/*
 * A334880  Numbers k such that gcd(k, k-th composite number) > 1.
 * https://oeis.org/A334880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = list of k such that gcd(k, c(k)) > 1 where c(k) is the
 *            k‑th composite number
 * Range:     u8 n <= 100, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_small.h"

/* Return the n‑th term of OEIS sequence A334880.
   The function stores previously computed terms in a static array
   and extends their range as needed. */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *terms = NULL;           /* stored a(k) values           */
    static size_t terms_cap = 0;          /* capacity of the array       */
    static size_t terms_len = 0;          /* number of terms currently    */
    static idx_t   last_i = 3;            /* last integer examined       */
    static idx_t   comp_cnt = 0;          /* number of composites seen   */

    /* enlarge the terms array if necessary */
    if (n > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        while (new_cap < n)
            new_cap *= 2;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        last_i++;                          /* next integer to check      */
        if (last_i < 4)                     /* composites start at 4       */
            continue;

        /* if last_i is not prime, it is a composite */
        if (!is_prime_small((seq_t)last_i)) {
            comp_cnt++;                     /* this is the k‑th composite */
            if (gcd((seq_t)comp_cnt, (seq_t)last_i) > (seq_t)1)
                terms[terms_len++] = (seq_t)comp_cnt;
        }
    }

    return terms[n - 1];
}
