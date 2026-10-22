/*
 * A365883  Numbers k whose least prime divisor is equal to its exponent in
 *          the prime factorization of k.
 * https://oeis.org/A365883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 9007, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A365883.
   a(n) is the n-th integer k (n ≥ 1) such that
   the least prime divisor of k equals its exponent in the prime factorisation of k. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Internal cache for the computed terms.                               */
/* --------------------------------------------------------------------- */
static seq_t *terms   = NULL;   /* array of already found terms */
static size_t terms_len = 0;    /* number of stored terms    */
static size_t terms_cap = 0;    /* capacity of the array     */

/*  The next integer to test for the property.                         */
static uint64_t next_candidate = 4;   /* first possible term is 4 */

/* --------------------------------------------------------------------- */
/*  Append one value to the cache, growing it if necessary.              */
/* --------------------------------------------------------------------- */
static void append_term(uint64_t v)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 8;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = (seq_t)v;
}

/* --------------------------------------------------------------------- */
/*  Test whether a given unsigned 64‑bit integer satisfies the property.*/
/* --------------------------------------------------------------------- */
static bool is_term(uint64_t k)
{
    /* Find the least prime divisor p of k. */
    uint64_t p = 0;
    uint64_t n = k;
    for (p = 2; p <= n / p; ++p) {   /* p*p <= n  without overflow */
        if (k % p == 0) {
            break;                 /* found least prime divisor */
        }
    }
    if (p > n / p) {                 /* n is prime, hence p = n */
        p = n;
    }

    /* Determine the exponent e of p in k. */
    uint64_t e = 0;
    while (k % p == 0) {
        k /= p;
        ++e;
    }

    return e == p;
}

/* --------------------------------------------------------------------- */
/*  Public sequence accessor.                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                   /* undefined for n=0, return zero gracefully */
    }

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        while (true) {
            if (is_term(next_candidate)) {
                append_term(next_candidate);
                ++next_candidate;    /* advance for the next round */
                break;               /* one more term added */
            }
            ++next_candidate;        /* not a term, test next number */
        }
    }
    return terms[n - 1];
}
