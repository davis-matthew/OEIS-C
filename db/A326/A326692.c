/*
 * A326692  Values of k for which the denominator of (Sum_{prime p | k} 1/p -
 *          1/k) is k.
 * https://oeis.org/A326692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that (sum_{p|k} 1/p – 1/k) has
 *            denominator k. Equivalent to: k is a term iff gcd(Σ(k/p) – 1, k)
 *            == 1 where the sum is over distinct prime divisors p of k
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* prime data (computed once) */
static uint64_t *primes = NULL;    /* pointers to primes */
static size_t   primes_cnt = 0;
static bool     primes_prepared = false;

/* cached sequence */
static seq_t *terms              = NULL;   /* array of terms */
static idx_t  terms_len   = 0;            /* number of terms stored */
static idx_t  terms_cap   = 0;            /* capacity of array */

/* next integer to test (kept between calls) */
static uint64_t next_k = 1;

/* enlarge the terms array to at least <need> capacity */
static void ensure_terms_cap(idx_t need)
{
    if (terms_cap >= need) return;

    idx_t new_cap = terms_cap ? terms_cap << 1 : 16;
    if (new_cap < need) new_cap = need;

    seq_t *new_terms = seq_alloc_new_array(new_cap);
    if (!new_terms) abort();

    if (terms) {
        memcpy(new_terms, terms, (size_t)terms_len * sizeof(seq_t));
        seq_free_array(terms);
    }
    terms = new_terms;
    terms_cap = new_cap;
}

/* test if k is a term of the sequence */
static bool is_term(uint64_t k)
{
    if (k == 1) return true;

    uint64_t temp = k;
    uint64_t factors[32];
    size_t   fcnt = 0;

    /* factorization: distinct prime divisors */
    for (size_t i = 0; i < primes_cnt &&
                       (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            factors[fcnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) factors[fcnt++] = temp;

    /* Σ(k/p) over distinct primes */
    seq_t kseq = (seq_t)k;
    seq_t sum = 0;
    for (size_t i = 0; i < fcnt; ++i)
        sum += kseq / (seq_t)factors[i];

    seq_t m = sum - (seq_t)1;
    seq_t g = gcd(m, kseq);
    return g == (seq_t)1;
}

/* return the nth term (1‑indexed) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* ensure prime list is ready */
    if (!primes_prepared) {
        /* an upper bound large enough for the first 10 000 terms */
        uint64_t limit = 2000000ULL;
        build_primes_upto(limit, &primes, &primes_cnt);
        primes_prepared = true;
    }

    /* grow storage if necessary */
    ensure_terms_cap(n);

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        if (is_term(next_k))
            terms[terms_len++] = (seq_t)next_k;
        ++next_k;
    }

    return terms[n - 1];
}
