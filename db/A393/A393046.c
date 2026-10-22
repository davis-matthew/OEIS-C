/*
 * A393046  Numbers whose smallest prime factor is not 5, while the least
 *          prime not dividing their arithmetic derivative is 5.
 * https://oeis.org/A393046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 2490, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A393046:
   Numbers whose smallest prime factor is not 5, while
   the least prime not dividing their arithmetic derivative is 5. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------  static data and helpers  ---------- */

static seq_t *terms = NULL;      /* array of found terms   */
static size_t terms_cap = 0;
static size_t terms_len = 0;
static uint64_t next_candidate = 2;   /* next integer to test */

static uint32_t *prime_vals = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static bool primes_ready = false;

/* Pre‑compute all primes up to 200 000 using a simple sieve. */
static void init_prime_sieve(void)
{
    if (primes_ready) return;
    const int LIMIT = 200000;
    bool *is_comp = (bool*)calloc((size_t)LIMIT + 1, sizeof(bool));
    if (!is_comp) return;

    prime_cap = 20000;          /* initial capacity – more than enough */
    prime_vals = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_vals) { free(is_comp); return; }

    for (int p = 2; p <= LIMIT; ++p) {
        if (!is_comp[p]) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(prime_vals, prime_cap * sizeof(uint32_t));
                if (!tmp) { free(is_comp); free(prime_vals); return; }
                prime_vals = tmp;
            }
            prime_vals[prime_cnt++] = (uint32_t)p;
            if ((int64_t)p * p <= LIMIT) {
                for (int64_t j = (int64_t)p * p; j <= LIMIT; j += p)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    primes_ready = true;
}

/* ----------  main sequence routine  ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Ensure prime list is ready. */
    init_prime_sieve();

    /* Allocate terms buffer if needed. */
    if (!terms) {
        terms_cap = 0;
        terms_len = 0;
        terms = NULL;
    }

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        uint64_t k = next_candidate;

        /* Factorise k using the pre‑computed prime list. */
        uint64_t m = k;
        uint64_t min_p = 0;
        const size_t MAX_FACTS = 64;   /* enough for any 64‑bit integer */
        uint64_t primes_arr[MAX_FACTS];
        uint64_t exps_arr[MAX_FACTS];
        size_t pf_cnt = 0;

        for (size_t i = 0; i < prime_cnt && (uint64_t)prime_vals[i] * (uint64_t)prime_vals[i] <= m; ++i) {
            uint64_t p = (uint64_t)prime_vals[i];
            if (m % p == 0) {
                if (min_p == 0) min_p = p;           /* smallest prime factor */
                uint64_t e = 0;
                while (m % p == 0) { m /= p; ++e; }
                primes_arr[pf_cnt] = p;
                exps_arr[pf_cnt] = e;
                ++pf_cnt;
            }
        }
        if (m > 1) {                                 /* remaining prime factor */
            uint64_t p = m;
            if (min_p == 0) min_p = p;
            primes_arr[pf_cnt] = p;
            exps_arr[pf_cnt] = 1;
            ++pf_cnt;
        }

        /* Check the smallest‑prime‑factor condition. */
        if (min_p != 5) {
            /* Compute the arithmetic derivative of k. */
            seq_t d = 0;
            for (size_t j = 0; j < pf_cnt; ++j) {
                d += ((seq_t)(k / primes_arr[j])) * (seq_t)exps_arr[j];
            }

            /* Test the derivative condition:
               divisible by 2 and 3, but not by 5. */
            if ((d % 2 == 0) && (d % 3 == 0) && (d % 5 != 0)) {
                /* Store the term. */
                if (terms_len == terms_cap) {
                    terms_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
                    seq_t *tmp = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;   /* allocation failure */
                    terms = tmp;
                }
                terms[terms_len++] = (seq_t)k;
            }
        }

        ++next_candidate;
    }

    return terms[n - 1];
}
