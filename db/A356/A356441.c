/*
 * A356441  Numbers k such that A309892(k) < k/gpf(k), where gpf = A006530;
 *          complement of A356438.
 * https://oeis.org/A356441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 9260, u32 n <= 9260, u64 n <= 9260,
 *            u128 n <= 9260, big any n
 * Helpers:   gpf, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: A356441 contains all positive integers k such that
 * k / gpf(k) > nextprime(gpf(k)),
 * where gpf(k) is the greatest prime factor of k, and nextprime(p) is
 * the smallest prime strictly greater than p.
 * The first term a(1) is 8.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gpf.h"          /* uint64_t gpf(uint64_t) */
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t) */

/* Returns the n‑th term (1‑based) of A356441. */
seq_t a(idx_t n)
{
    /* static state to memoise the sequence */
    static seq_t *terms = NULL;   /* array of terms */
    static size_t terms_len = 0;  /* how many terms stored */
    static size_t terms_cap = 0;  /* allocated capacity */
    static uint64_t next_k = 8;   /* next integer to examine */

    /* lazily initialise the cache */
    if (terms == NULL) {
        terms_cap = 32;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (terms == NULL) exit(1);                /* allocation failure */
    }

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        uint64_t g = gpf(next_k);                 /* greatest prime factor */
        seq_t   k_seq = (seq_t)next_k;             /* cast to seq_t */
        seq_t   g_seq = (seq_t)g;                 /* cast to seq_t */

        /* compute k / gpf(k) */
        seq_t k_div_g = k_seq / g_seq;

        /* compute next prime after g */
        uint64_t nextp_uint = next_prime(g);
        seq_t   nextp_seq = (seq_t)nextp_uint;

        if (k_div_g > nextp_seq) {
            /* enlarge storage if necessary */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (tmp == NULL) exit(1);          /* reallocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = k_seq;
        }
        ++next_k;
    }

    /* return the requested 1‑based term */
    return terms[n - 1];
}
