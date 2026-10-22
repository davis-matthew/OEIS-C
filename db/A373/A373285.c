/*
 * A373285  Numbers k that are composite and not a powers of a prime k such
 *          that sopf^{h+1}(k) divides sopf^{h}(k), with sopf^{0}(k)=k, for
 *          h=0..A321944(k)-1, where sopf^{h} is the h-th iteration of sopf
 *          and sopf = A008472.
 * https://oeis.org/A373285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number k that are composite and not a powers of a prime k
 *            such that sopf^{h+1}(k) divides sopf^{h}(k), with sopf^{0}(k)=k,
 *            for h=0..A321944(k)-1
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Static storage for all terms that have been generated so far. */
static seq_t *terms = NULL;
static size_t terms_cap = 0;     /* allocated capacity */
static idx_t terms_filled = 0;   /* number of terms stored */
static uint64_t candidate = 2;   /* next integer to test */

/* Return true if k satisfies the definition of the sequence. */
static bool satisfies(uint64_t k)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* Factor k once, obtaining all distinct prime factors. */
    factor_u64(k, primes, exps, &cnt);

    /* Composite number must have at least two distinct prime factors. */
    if (cnt < 2)
        return false;

    /* First iteration: sopf(k) = sum of its distinct prime factors. */
    seq_t cur = 0;
    for (size_t i = 0; i < cnt; ++i)
        cur += (seq_t)primes[i];

    /* Check that sopf(k) divides k. */
    if ((seq_t)k % cur != 0)
        return false;

    seq_t prev_sum = cur;

    /* Iterate further divisions until the number of distinct prime factors is exhausted. */
    for (size_t h = 1; h < cnt; ++h) {
        uint64_t val = (uint64_t)prev_sum;
        factor_u64(val, primes, exps, &cnt);

        seq_t next_sum = 0;
        for (size_t i = 0; i < cnt; ++i)
            next_sum += (seq_t)primes[i];

        if (prev_sum % next_sum != 0)
            return false;

        prev_sum = next_sum;
    }

    return true;
}

/* Ensure that the terms array has room for at least 'needed' elements. */
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap)
        return;

    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* Main function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined below offset */
        return 0;

    while ((idx_t)terms_filled < n) {
        if (satisfies(candidate)) {
            grow_terms(terms_filled + 1);
            terms[terms_filled++] = candidate;
        }
        ++candidate;
    }
    return terms[n - 1];
}
