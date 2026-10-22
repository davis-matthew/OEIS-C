/*
 * A330477  Semiprimes (A001358) p*q such that p*q+p+q is also a semiprime.
 * https://oeis.org/A330477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 6514, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A330477: return the nth semiprime p*q such that p*q+p+q is also a semiprime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Append a new term to the dynamic array */
static void add_term(seq_t **terms, size_t *len, size_t *cap, seq_t val)
{
    if (!*terms || *len >= *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *terms = tmp;
        *cap = new_cap;
    }
    (*terms)[*len] = val;
    (*len)++;
}

/* Return 1 and set *p0,*p1 if n is a semiprime, otherwise return 0. */
static int get_semiprime_factors(uint64_t n, uint64_t *p0, uint64_t *p1)
{
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    size_t mult = 0;
    for (size_t i = 0; i < cnt; i++) mult += ex[i];
    if (mult != 2) return 0;

    if (cnt == 1) {                /* perfect square of a prime */
        *p0 = *p1 = pr[0];
    } else {                       /* product of two distinct primes */
        *p0 = pr[0];
        *p1 = pr[1];
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, guard just in case */

    /* static state across calls */
    static seq_t *seq_terms = NULL;
    static size_t seq_size  = 0, seq_cap  = 0;
    static uint64_t candidate = 1;

    /* generate terms until we have enough */
    while (seq_size < n) {
        candidate++;                /* next integer to test */

        uint64_t p, q;
        if (!get_semiprime_factors(candidate, &p, &q))
            continue;              /* candidate is not a semiprime */

        uint64_t m = candidate + p + q;
        uint64_t dummy0, dummy1;
        if (!get_semiprime_factors(m, &dummy0, &dummy1))
            continue;              /* m is not a semiprime */

        /* candidate satisfies the condition */
        add_term(&seq_terms, &seq_size, &seq_cap, (seq_t)candidate);
    }

    return seq_terms[n - 1];
}
