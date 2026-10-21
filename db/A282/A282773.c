/*
 * A282773  Numbers k such that Bernoulli number B_{k} has denominator 498.
 * https://oeis.org/A282773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 95, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282773: Numbers k such that the denominator of the
   Bernoulli number B_k is 498.
   Formula:  k is a multiple of 82 and no prime p other than 2,3,83
   satisfies (p-1) | k. */
/* Implementation computes all valid k ≤ LIMIT (20 000 000) once,
   stores them in a static array, and answers a(n) by array lookup. */

#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 20000000ULL   /* upper bound for precomputation (≥ all needed k) */

static bool *is_prime   = NULL;   /* sieve of Eratosthenes */
static bool *invalid    = NULL;   /* marks k that are forbidden (k% (p-1)==0 for some p) */
static seq_t *terms     = NULL;   /* sequence values */
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* append a new term to the dynamic array */
static void append_term(seq_t v)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);            /* unrecoverable allocation failure */
        terms = new_arr;
        terms_cap = new_cap;
    }
    terms[terms_len++] = v;
}

/* initialise primes and the forbidden‑mask, then generate the sequence */
static void init_precomputation(void)
{
    if (is_prime) return;            /* already done */

    /* allocate and initialise sieving arrays */
    is_prime  = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    invalid   = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime || !invalid) exit(1);

    for (uint64_t i = 0; i <= LIMIT; ++i) {
        is_prime[i] = true;
        invalid[i]  = false;
    }
    is_prime[0] = is_prime[1] = false;

    /* Eratosthenes sieve to find all primes up to LIMIT */
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* For each prime p > 3 and p ≠ 83, mark multiples of (p-1) as forbidden */
    for (uint64_t p = 5; p <= LIMIT; ++p) {
        if (!is_prime[p]) continue;
        if (p == 83) continue;        /* p = 83 is allowed; skip its (p-1) = 82 */
        uint64_t step = p - 1;
        for (uint64_t k = step; k <= LIMIT; k += step)
            invalid[k] = true;
    }

    /* Generate all valid multiples of 82 and store them */
    for (uint64_t k = 82; k <= LIMIT; k += 82) {
        if (!invalid[k])
            append_term((seq_t)k);
    }
}

/* sequence function – returns the n‑th a(n), 1‑based */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* offset is 1 */

    init_precomputation();                /* ensure tables are built */

    if ((idx_t)terms_len < n)            /* requested n exceeds precomputed range */
        return (seq_t)0;                  /* outside bounds – unlikely for n ≤ 10000 */

    return terms[n - 1];
}
