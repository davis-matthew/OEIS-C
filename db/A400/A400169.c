/*
 * A400169  Numbers k such that A235224(A003415(k)) = A400164(k), where
 *          A235224 is the length of primorial base representation, A400164(n)
 *          = Max_{d|n, d is a prime power} A235224(n/d), and A003415 is the
 *          arithmetic derivative.
 * https://oeis.org/A400169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 182, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A400169: numbers k such that
   A235224(A003415(k)) = A400164(k),
   where A235224(n) is the length of the primorial base
   representation of n,
   A400164(k) = max_{d|k, d is a prime power} A235224(k/d),
   and A003415 is the arithmetic derivative. 
   The function returns the n-th term (offset 1). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* utilities provided by the framework */
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

static uint64_t *prime_list = NULL;   /* array of primes */
static size_t   prime_cnt   = 0;       /* how many primes */

/* initialise the prime list with a generous upper bound */
static void init_primes(void)
{
    if (prime_list == NULL) {
        /* 2 000 000 is far more than enough for all k needed for n≤10 000 */
        build_primes_upto(2000000, &prime_list, &prime_cnt);
    }
}

/* --------- primorial base length ------------------------------------- */
/* smallest i such that prod_{j=1..i} p_j > n   (i.e. n has i digits)   */
static seq_t primorial_length(seq_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prod = 1;
    size_t i   = 0;
    while (prod <= n) {
        seq_t p = (seq_t)prime_list[i++];
        if (prod > n / p) {   /* would overflow or already exceed n */
            prod = n;         /* force termination of loop */
            break;
        }
        prod *= p;
    }
    return (seq_t)i;
}

/* --------- arithmetic derivative ------------------------------------- */
static seq_t arithmetic_derivative(idx_t k)
{
    if (k <= 1) return 0;

    uint64_t pr[64], ex[64];
    size_t   cnt;
    factor_u64((uint64_t)k, pr, ex, &cnt);

    seq_t der = 0;
    for (size_t i = 0; i < cnt; ++i) {
        der += ((seq_t)k * (seq_t)ex[i]) / (seq_t)pr[i];
    }
    return der;
}

/* --------- A400164 ---------------------------------------------------- */
static seq_t A400164(idx_t k)
{
    if (k <= 1) return 0;

    uint64_t pr[64], ex[64];
    size_t   cnt;
    factor_u64((uint64_t)k, pr, ex, &cnt);

    seq_t maxlen = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        seq_t pow = 1;
        for (uint64_t j = 1; j <= e; ++j) {
            pow *= (seq_t)p;
            seq_t m   = ((seq_t)k) / pow;
            seq_t len = primorial_length(m);
            if (len > maxlen) maxlen = len;
        }
    }
    return maxlen;
}

/* --------- check one k ---------------------------------------------- */
static bool satisfies(idx_t k)
{
    seq_t der  = arithmetic_derivative(k);
    seq_t len1 = primorial_length(der);
    seq_t len2 = A400164(k);
    return len1 == len2;
}

/* --------- memoised list of terms ------------------------------------ */
static seq_t *terms   = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static idx_t  current_k = 0;   /* largest k examined so far */

static void append_term(idx_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        terms      = tmp;
        terms_cap  = new_cap;
    }
    terms[terms_len++] = (seq_t)val;
}

/* --------- the public sequence function ------------------------------ */
seq_t a(idx_t n)
{
    /* initialise once */
    static bool init = false;
    if (!init) {
        init_primes();
        init = true;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        ++current_k;
        if (satisfies(current_k))
            append_term(current_k);
    }

    return terms[n - 1];
}
