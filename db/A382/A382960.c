/*
 * A382960  Numbers k such that k < A053669(k)^2 * A380539(k)^2, i.e., k <
 *          A382767(k).
 * https://oeis.org/A382960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = the nth positive integer k such that k < (smallest prime
 *            not dividing k)^2 * (second smallest prime not dividing k)^2
 *            (OEIS A382960)
 * Range:     u8 none, u16 n <= 610, u32 n <= 626, u64 n <= 626, u128 n <= 626,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Comparator for seq_t values */
static int seq_cmp(const void *p1, const void *p2) {
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* Dynamic array that will contain all terms of the sequence */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Ensure the dynamic array has room for at least `needed` elements */
static void ensure_terms_cap(size_t needed) {
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();
    terms = new_terms;
    terms_cap = new_cap;
}

/* Build the list of primes up to PRIME_LIMIT */
#define PRIME_LIMIT 2000

static void build_primes(uint32_t *primes, size_t *cnt) {
    bool is_comp[PRIME_LIMIT + 1] = {false};
    size_t index = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[(index)++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    *cnt = index;
}

/* Compute all terms of A382960 once and store them in `terms` */
static void init_terms(void) {
    if (terms_len) return;  /* already computed */

    /* 1. Prime table */
    uint32_t primes[500];   /* enough for primes <= 2000 */
    size_t primes_cnt;
    build_primes(primes, &primes_cnt);

    /* 2. Enumerate pairs (p,q) with p < q */
    for (size_t ip = 0; ip < primes_cnt; ++ip) {
        uint64_t p = primes[ip];
        for (size_t iq = ip + 1; iq < primes_cnt; ++iq) {
            uint64_t q = primes[iq];

            /* limit = p^2 * q^2 */
            __uint128_t limit = (__uint128_t)p * p * q * q;

            /* L = product of all primes < q, except p */
            __uint128_t L = 1;
            for (size_t r = 0; r < iq; ++r) {
                if (r == ip) continue;   /* skip p */
                L *= primes[r];
                if (L >= limit) break;
            }
            if (L >= limit) continue;   /* no k possible for this pair */

            /* t ranges from 1 to floor((limit-1)/L) */
            uint64_t t_max = (uint64_t)((limit - 1) / L);
            for (uint64_t t = 1; t <= t_max; ++t) {
                /* k = L * t */
                /* Exclude k if p or q divides t (would make k divisible by p or q) */
                if ((t % p == 0) || (t % q == 0)) continue;
                __uint128_t k128 = L * t;          /* guaranteed < limit */
                seq_t k = (seq_t)k128;             /* cast to output type */

                /* Store term */
                ensure_terms_cap(terms_len + 1);
                terms[terms_len++] = k;
            }
        }
    }

    /* 3. Sort terms */
    qsort(terms, terms_len, sizeof(seq_t), seq_cmp);

    /* 4. Deduplicate (safety) */
    size_t uniq = 0;
    for (size_t i = 0; i < terms_len; ++i) {
        if (i == 0 || seq_cmp(&terms[i], &terms[i - 1]) != 0) {
            terms[uniq++] = terms[i];
        }
    }
    terms_len = uniq;
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety, offset is 1 */
    init_terms();          /* compute on first use */

    if ((idx_t)(n - 1) < (idx_t)terms_len)
        return terms[n - 1];
    return 0;              /* sequence is finite; return 0 for indices beyond the last term */
}
