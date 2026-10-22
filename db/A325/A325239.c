/*
 * A325239  Irregular triangle read by rows where row 1 is {1} and row n > 1
 *          is the sequence starting with n and repeatedly applying A181819
 *          until 2 is reached.
 * https://oeis.org/A325239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 989, u16 n <= 9229, u32 n <= 9229, u64 n <= 9229,
 *            u128 n <= 9229, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325239: Irregular triangle read by rows; row n starts with n and
 * repeatedly applies A181819 until 2 is reached. This function returns
 * the flattened sequence. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"

/* Compute A181819 for a 64‑bit integer.  The result fits comfortably
 * in uint64_t for the sizes of rows used in this sequence (up to about
 * 10 000).  The product is taken in uint64_t; each step uses the
 * e‑th prime where e is the exponent of a prime factor of n. */
static uint64_t a181819_uint64(uint64_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;
    factor_u64(n, primes, exps, &count);

    uint64_t res = 1;
    for (size_t i = 0; i < count; ++i) {
        uint64_t e = exps[i];
        /* e is a small natural number; get_prime returns the e‑th prime. */
        uint32_t p = get_prime((idx_t)e);
        res *= p;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t current_row = 1;   /* last processed row number */

    /* initialise with a(1) = 1. */
    if (terms_len == 0) {
        terms_cap = 1;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms[0] = (seq_t)1;
        terms_len = 1;
    }

    /* build rows until we reach at least n terms. */
    while (terms_len < n) {
        ++current_row;          /* the next row number */

        uint64_t val = (uint64_t)current_row;
        for (;;) {
            if (terms_len + 1 > terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)val;

            if (val == 2)
                break;

            val = a181819_uint64(val);
        }
    }

    return terms[n - 1];
}
