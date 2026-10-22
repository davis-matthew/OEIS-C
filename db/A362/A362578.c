/*
 * A362578  Prime numbers followed by two consecutive numbers which are
 *          products of four distinct primes (or tetraprimes).
 * https://oeis.org/A362578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 24, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   count_distinct_primes, is_prime_big, is_squarefree, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A362578: a(n) is the nth prime p such that
 * p+1 and p+2 are products of four distinct primes (tetraprimes). */
/* The implementation builds the sequence on demand, memoizing terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/seq_realloc.h"

/* static cache for the sequence terms */
static seq_t *terms   = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t next_candidate = 2;

/* ensure the cache can hold at least new_cap elements */
static void ensure_terms_cap(size_t new_cap)
{
    if (new_cap <= terms_cap)
        return;
    size_t new_cap_act = terms_cap ? terms_cap << 1 : 16;
    if (new_cap_act < new_cap)
        new_cap_act = new_cap;
    seq_t *tmp = seq_realloc(terms, terms_cap, new_cap_act);
    if (!tmp)
        abort();   /* out‑of‑memory */
    terms = tmp;
    terms_cap = new_cap_act;
}

/* append a new term to the cache */
static void push_term(seq_t val)
{
    ensure_terms_cap(terms_len + 1);
    terms[terms_len++] = val;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        /* skip even numbers greater than 2 */
        if (next_candidate > 2 && (next_candidate & 1ULL) == 0) {
            ++next_candidate;
            continue;
        }

        if (is_prime_big((seq_t)next_candidate)) {
            uint64_t a1 = (uint64_t)next_candidate + 1;
            uint64_t a2 = (uint64_t)next_candidate + 2;

            if (is_squarefree((seq_t)a1) &&
                count_distinct_primes(a1) == 4 &&
                is_squarefree((seq_t)a2) &&
                count_distinct_primes(a2) == 4) {
                push_term((seq_t)next_candidate);
            }
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
