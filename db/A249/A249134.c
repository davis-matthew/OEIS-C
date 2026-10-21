/*
 * A249134  Numbers k such that Bernoulli number B_k has denominator 2730.
 * https://oeis.org/A249134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 320, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the denominator of Bernoulli number B_k is 2730.
 * Condition: k is a multiple of 12 and for every prime p>13 we have (p-1) ∤ k.
 * The sequence values are the k's that satisfy this condition. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Parameters for the sieve (maximum value needed for n up to about 10000). */
static const size_t SIEVE_LIMIT = 10000000; /* 10 million, ≈ 40 MB */

/* Cached data for the sequence */
static seq_t *term_cache = NULL;
static size_t term_count = 0;
static size_t term_cap   = 0;

/* Cached data for the sieve */
static uint32_t *spf = NULL;            /* smallest prime factor for each number */
static bool sieve_initialized = false;

/*--------------------------------------------------------------------------*/
/* Simple linear sieve to fill spf array.                                   */
/*--------------------------------------------------------------------------*/
static void init_sieve(void)
{
    if (sieve_initialized)
        return;
    sieve_initialized = true;

    size_t N = SIEVE_LIMIT;
    spf = (uint32_t*)malloc((N + 1) * sizeof(uint32_t));
    if (!spf) return; /* let program crash if OOM */

    for (size_t i = 0; i <= N; ++i)
        spf[i] = 0;

    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= N / i) {
                for (size_t j = i * i; j <= N; j += i) {
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
                }
            }
        }
    }
    spf[0] = 1;
    spf[1] = 1;
}

/*--------------------------------------------------------------------------*/
/* Return true if divisor+1 is a prime > 13.                                 */
/*--------------------------------------------------------------------------*/
static bool divisor_check(uint32_t d)
{
    uint32_t p = d + 1U;
    if (p <= 13U)
        return false;
    if (p >= SIEVE_LIMIT) /* should not happen with the chosen limit */
        return false;
    return spf[p] == p;   /* prime test via spf */
}

/*--------------------------------------------------------------------------*/
/* Factorize n using the precomputed spf array.                               */
/*--------------------------------------------------------------------------*/
#define MAX_FACTORS 10   /* enough for n up to 10^10, here only up to 10M */

static void factorize(uint32_t n, uint32_t *p, int *e, int *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint32_t prime = spf[n];
        int exp = 0;
        while (n % prime == 0) {
            n /= prime;
            ++exp;
        }
        p[*cnt] = prime;
        e[*cnt] = exp;
        ++(*cnt);
    }
}

/*--------------------------------------------------------------------------*/
/* Enumerate all divisors from factorization and check them.                 */
/*--------------------------------------------------------------------------*/
static bool check_candidate(uint32_t k)
{
    uint32_t fac[MAX_FACTORS];
    int exp[MAX_FACTORS];
    int num_factors;
    factorize(k, fac, exp, &num_factors);

    /* Generate divisors iteratively.  We keep the list in divs array.     */
    static uint32_t divs[5000]; /* safe upper bound for 10M */
    size_t dcnt = 1;
    divs[0] = 1U;

    for (int i = 0; i < num_factors; ++i) {
        uint32_t prime = fac[i];
        int   exp_cnt = exp[i];
        size_t old_dc = dcnt;

        /* powers of prime: prime^1 ... prime^exp_cnt */
        uint32_t mult = 1U;
        for (int e = 1; e <= exp_cnt; ++e) {
            mult *= prime;
            for (size_t j = 0; j < old_dc; ++j) {
                uint32_t val = divs[j] * mult;
                divs[dcnt++] = val;
            }
        }
    }

    for (size_t i = 0; i < dcnt; ++i) {
        if (divisor_check(divs[i]))
            return false;               /* undesirable prime found */
    }
    return true;                        /* all primes checked are in {2,3,5,7,13} */
}

/*--------------------------------------------------------------------------*/
/* Generate terms up to index n (1‑based). Done lazily with static state.     */
/*--------------------------------------------------------------------------*/
static void generate_up_to(idx_t n)
{
    init_sieve();

    static idx_t next_multiple = 12; /* start from the first candidate */

    while (term_count < n) {
        uint32_t cand = (uint32_t)next_multiple;

        if (cand <= SIEVE_LIMIT && check_candidate(cand)) {
            /* store */
            if (term_count == term_cap) {
                size_t new_cap = term_cap == 0 ? 8 : term_cap * 2;
                seq_t *p = (seq_t*)realloc(term_cache, new_cap * sizeof(seq_t));
                if (!p) exit(1); /* OOM */
                term_cache = p;
                term_cap   = new_cap;
            }
            term_cache[term_count] = (seq_t)cand;
            ++term_count;
        }
        next_multiple += 12U;
    }
}

/*--------------------------------------------------------------------------*/
/* Public function required by the test harness.                              */
/*--------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but return 0 for safety */

    generate_up_to(n);
    return term_cache[n - 1];
}
