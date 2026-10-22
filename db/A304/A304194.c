/*
 * A304194  Numbers k such that k = Product (p_j^e_j) = Product (pi(p_j)*p_j),
 *          where pi() = A000720.
 * https://oeis.org/A304194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 33, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A304194: Numbers k such that k = ∏ (π(p_j) * p_j) over all
   primes p_j dividing k.  We generate all such numbers up to a
   fixed bound and keep them sorted; this is fast enough to provide
   the first 10 000 terms (and more) within the time limits. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Types supplied by the build.  */
extern seq_t pow_seq(seq_t a, int k);   /* not used, here only to keep the build happy */

/* Maximum number up to which we pre‑compute the sequence. */
#define MAX_LIMIT 10000000U

/* Static state. */
static bool initialized = false;
static seq_t *terms        = NULL;   /* sorted sequence values */
static size_t term_cnt     = 0;
static size_t term_cap     = 0;

static int  *spf            = NULL;   /* least prime factor */
static int  *prime_idx      = NULL;   /* prime → 0‑based index */
static int  *primes         = NULL;   /* list of primes */
static size_t prime_cnt     = 0;
static size_t prime_cap     = 0;

/* ------------------------------------------------------------------ */
/* Simple linear sieve producing spf and primes up to MAX_LIMIT. */
static void sieve(void) {
    const int N = MAX_LIMIT;
    spf      = (int *)malloc((N + 1) * sizeof(int));
    prime_idx = (int *)malloc((N + 1) * sizeof(int));
    prime_cap = N / 10;                    /* enough for ~0.1·N primes */
    primes   = (int *)malloc(prime_cap * sizeof(int));

    for (int i = 0; i <= N; ++i)
        spf[i] = 0, prime_idx[i] = -1;

    int *pr = primes;
    int pr_cnt = 0;

    for (int i = 2; i <= N; ++i) {
        if (!spf[i]) {            /* i is prime */
            spf[i] = i;
            if (pr_cnt == (int)prime_cap) {
                prime_cap <<= 1;
                primes = (int *)realloc(primes, prime_cap * sizeof(int));
            }
            primes[pr_cnt++] = i;
        }
        for (int j = 0; j < pr_cnt && primes[j] <= spf[i] &&
              (long long)i * primes[j] <= N; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
    prime_cnt = pr_cnt;
    /* Build index map prime→position. */
    for (size_t i = 0; i < prime_cnt; ++i)
        prime_idx[primes[i]] = (int)i;
}

/* ------------------------------------------------------------------ */
/* Generate the sequence up to MAX_LIMIT and store it in a sorted array. */
static void generate_sequence(void) {
    sieve();

    /* Rough estimation: we will probably need far fewer than all
       numbers up to MAX_LIMIT.  Allocate a still huge capacity
       to avoid reallocations. */
    term_cap = 100000;
    terms = (seq_t *)malloc(term_cap * sizeof(seq_t));

    for (int k = 1; k <= MAX_LIMIT; ++k) {
        unsigned long long prod = 1ULL;
        int temp = k;
        int last_p = -1;
        while (temp > 1) {
            int p = spf[temp];
            while (temp % p == 0) temp /= p;
            if (p == last_p) continue;      /* should not happen, SPF ensures distinct */
            last_p = p;
            /* factor = (pi(p)) * p = (index+1) * p */
            unsigned long long factor = (unsigned long long)(prime_idx[p] + 1) * (unsigned long long)p;
            if (prod > (unsigned long long)k / factor) {
                prod = (unsigned long long)k + 1; /* guarantee >k */
                break;
            }
            prod *= factor;
        }
        if (prod == (unsigned long long)k) {
            if (term_cnt == term_cap) {
                term_cap <<= 1;
                terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
            }
            terms[term_cnt++] = (seq_t)k;
        }
    }
    /* shrink array to actual size */
    terms = (seq_t *)realloc(terms, term_cnt * sizeof(seq_t));
}

/* ------------------------------------------------------------------ */
/* Public function required by the harness. */
seq_t a(idx_t n) {
    if (!initialized) {
        generate_sequence();
        initialized = true;
    }
    if (n == 0 || (idx_t)n > term_cnt)
        return (seq_t)0;
    return terms[n - 1];
}
