/*
 * A376205  Prime indices of triangular numbers k such that k + 1 and k - 2
 *          are not prime.
 * https://oeis.org/A376205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p such that the triangular number * T(p) =
 *            p·(p+1)/2 satisfies (T(p)+1) and (T(p)-2) are not prime
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 35, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"      /* provides bool is_prime(idx_t) */
#include "../utils/is_prime_big.h"  /* provides bool is_prime_big(seq_t) */

/* Return the nth term of OEIS sequence A376205. */
seq_t a(idx_t n)
{
    static seq_t *seq      = NULL;   /* array of found prime indices            */
    static size_t seq_cap = 0;       /* allocated capacity                      */
    static size_t seq_len = 0;       /* number of terms stored (seq_len >= n needed) */
    static idx_t  next_prime_candidate = 2;  /* next integer to test for primality */

    /* Extend the stored sequence if necessary. */
    while (seq_len < n) {
        /* Find the next prime candidate. */
        while (!is_prime(next_prime_candidate))
            ++next_prime_candidate;

        /* Compute the triangular number T(p) = p * (p + 1) / 2. */
        seq_t p = (seq_t)next_prime_candidate;
        seq_t t = p * (p + 1) / 2;

        /* Test the conditions on T(p)+1 and T(p)-2. */
        bool bad = false;
        if (is_prime_big(t + 1))
            bad = true;
        else if (is_prime_big(t - 2))
            bad = true;

        /* If both are composite, store the prime index. */
        if (!bad) {
            /* Ensure capacity of the array. */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) return 0;     /* allocation failure (unlikely in test) */
                seq      = tmp;
                seq_cap  = new_cap;
            }
            seq[seq_len++] = p;
        }

        /* Proceed to the next number. */
        ++next_prime_candidate;
    }

    /* Return the nth term (1‑based index). */
    return seq[n - 1];
}
