/*
 * A343197  Numbers k such that A025281(k) is prime.
 * https://oeis.org/A343197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 224, u32 n <= 63, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A343197
 *  Implementation note:
 *  A025281(k) = sum_{p <= k} p * v_p(k!)
 *  where v_p(k!) is the exponent of prime p in k!
 *  This function returns the n-th integer k such that A025281(k) is prime.
 *  It increments k, updates the cumulative sum S(k) incrementally,
 *  and checks primality using the supplied is_prime_big routine.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"

/* ---------- Cached data ---------- */
static seq_t *terms      = NULL; /* stored sequence values */
static size_t terms_cap  = 0;    /* capacity of terms array */
static size_t terms_len  = 0;    /* number of terms stored */

/* Smallest prime factor table */
static uint64_t *spf      = NULL;
static const idx_t SPF_BOUND = 500000; /* safety bound for the sieve */

/* current state */
static idx_t current_k = 0;          /* last processed k (starting from 0) */
static seq_t current_sum = (seq_t)0; /* S(k) = sum_{p <= k} p * v_p(k!) */

/* ---------- Helper: ensure capacity for terms array ---------- */
static void ensure_terms_capacity(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms)
        abort();
    terms = new_terms;
    terms_cap = new_cap;
}

/* ---------- Simple linear sieve to fill spf up to SPF_BOUND ---------- */
static void init_sieve(void)
{
    if (spf) /* already built */
        return;
    spf = (uint64_t *)calloc((size_t)SPF_BOUND + 1, sizeof(uint64_t));
    if (!spf)
        abort();

    uint64_t *prime_list = (uint64_t *)malloc((size_t)SPF_BOUND * sizeof(uint64_t));
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= SPF_BOUND; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            prime_list[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt &&
             prime_list[j] <= spf[i] &&
             i * (idx_t)prime_list[j] <= SPF_BOUND; ++j) {
            spf[i * (idx_t)prime_list[j]] = prime_list[j];
        }
    }
    free(prime_list);
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, return 0 for safety */

    init_sieve();           /* ensure spf table is ready */

    /* Initialise terms array lazily */
    if (!terms) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms)
            abort();
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        ++current_k;
        if (current_k > SPF_BOUND)
            abort(); /* exceeded pre‑computed bound – unlikely for tests */

        /* Factorise current_k and update cumulative sum */
        idx_t x = current_k;
        while (x > 1) {
            uint64_t p = spf[x];
            uint64_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            current_sum += (seq_t)p * (seq_t)cnt;
        }

        /* If S(k) is prime, record k */
        if (is_prime_big(current_sum)) {
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = current_k;
        }
    }

    return terms[n - 1];
}
