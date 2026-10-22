/*
 * A342190  Numbers k such that A001065(k) = sigma(k) - k is the sum of 2
 *          squares.
 * https://oeis.org/A342190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A342190 – numbers k such that A001065(k)=sigma(k)-k is a sum of
 *            two squares
 * Range:     u8 n <= 138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declarations (seq_t, idx_t are supplied by the build system) */
static seq_t *seq_terms = NULL;
static size_t   terms_len   = 0;
static size_t   terms_cap   = 0;

static uint32_t *spf      = NULL;   /* smallest prime factor array */
static uint64_t  spf_limit = 0;     /* maximum index that spf currently covers */

static uint32_t *prime_list = NULL; /* list of primes up to spf_limit */
static size_t   prime_count  = 0;
static size_t   prime_cap    = 0;

/* ------------------------------------------------------------------ */
/* Grow the prime list array if needed */
static void grow_primes(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    prime_list = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    prime_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Build the linear sieve for spf up to 'limit'.  Also stores primes. */
static void build_spf(uint64_t limit)
{
    free(spf);
    free(prime_list);

    spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!spf) abort();

    spf_limit = limit;
    prime_count = 0;
    prime_cap = 0;
    prime_list = NULL;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            grow_primes(prime_count + 1);
            prime_list[prime_count++] = (uint32_t)i;
        }
        uint32_t spfi = spf[i];
        for (size_t j = 0; j < prime_count && prime_list[j] <= spfi; ++j) {
            uint64_t prod = i * (uint64_t)prime_list[j];
            if (prod > limit) break;
            spf[prod] = prime_list[j];
        }
    }
}

/* ------------------------------------------------------------------ */
/* Compute σ(k) using the pre‑computed spf array. */
static uint64_t sigma_of(uint64_t k)
{
    uint64_t res = 1;
    uint64_t tmp = k;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        uint64_t term = 1;
        uint64_t power = 1;
        while (tmp % p == 0) {
            tmp /= p;
            power *= (uint64_t)p;
            term += power;
        }
        res *= term;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Check whether m >= 0 can be written as a sum of two squares. */
static bool is_two_squares(uint64_t m)
{
    if (m == 0) return true;   /* 0 = 0^2 + 0^2 */

    uint64_t rem = m;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= rem; ++i) {
        uint32_t p = prime_list[i];
        if (rem % p == 0) {
            int exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            if ((p & 3) == 3 && (exp & 1)) return false;
        }
    }
    if (rem > 1) {  /* remaining prime factor */
        if ((rem & 3) == 3) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Grow the terms array if needed. */
static void grow_terms(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_terms = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main function requested by the problem statement. */
seq_t a(idx_t n)  /* n is 1‑based: a(1) returns the first term */
{
    if (n == 0) return (seq_t)0;           /* undefined, but safe */

    /* Ensure we have at least an initial sieve. */
    if (spf_limit == 0) build_spf(100000);

    /* Return previously computed term, if available. */
    if ((size_t)n <= terms_len) return seq_terms[(size_t)n - 1];

    /* Tracking the maximum k already examined. */
    static uint64_t last_checked = 0;

    for (uint64_t k = last_checked + 1; ; ++k) {
        /* Extend sieve if necessary. */
        if (k > spf_limit) {
            uint64_t new_limit = spf_limit ? spf_limit << 1 : 1;
            while (new_limit < k) new_limit <<= 1;
            build_spf(new_limit);
        }

        uint64_t sigma = sigma_of(k);
        uint64_t m = sigma - k;

        if (is_two_squares(m)) {
            grow_terms(terms_len + 1);
            seq_terms[terms_len++] = (seq_t)k;
            if (terms_len == (size_t)n) {
                last_checked = k;
                return seq_terms[(size_t)n - 1];
            }
        }
    }
}
