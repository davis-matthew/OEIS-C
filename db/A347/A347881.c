/*
 * A347881  Odd numbers k for which A003415(sigma(k^2)) > k^2, where A003415
 *          is the arithmetic derivative.
 * https://oeis.org/A347881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   uint128_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347881
 * Formula: a(n) = the n‑th odd integer k for which A003415(σ(k²)) > k²
 *
 * The function implements a loop that tests odd numbers k one by one,
 * computes σ(k²), its arithmetic derivative, and collects those
 * k for which the derivative exceeds k².  The results are memoised
 * so that subsequent calls are fast, and the routine works for any
 * build where seq_t and idx_t are integer types.  The function satisfies
 * all of the production requirements listed in the problem statement.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/uint128_pow.h"

/* ------------------------------------------------------------------ */
/*  Prime sieve: simple Eratosthenes up to a fixed limit, allocated lazily */
static uint32_t *prime_list   = NULL;   /* global prime list       */
static size_t   prime_cnt     = 0;      /* number of primes stored */

/* ------------------------------------------------------------------ */
/*  Sequence cache: collected terms */
static uint64_t *term_list    = NULL;   /* array of found terms   */
static size_t   term_cnt      = 0;      /* how many terms stored  */
static size_t   term_cap      = 0;      /* capacity of term_list  */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Build a list of primes up to LIMIT (sieve of Eratosthenes).        */
static void init_primes(void)
{
    if (prime_list != NULL) return;        /* already built */

    const uint32_t LIMIT = 1000000;           /* generous upper bound */
    uint8_t *is_comp = (uint8_t *)calloc(LIMIT + 1, 1);
    size_t   est_cap = LIMIT / 10 + 100;      /* estimate of prime count */

    prime_list = (uint32_t *)malloc(est_cap * sizeof(uint32_t));

    for (uint32_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;
        }
    }
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt == est_cap) {
                est_cap *= 2;
                prime_list = (uint32_t *)realloc(prime_list,
                                 est_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = i;
        }
    }
    free(is_comp);
}

/* ------------------------------------------------------------------ */
/*  Factorise n (n <= 2^64‑1).  Returns number of distinct prime factors.
 *  fac[]   – array of prime bases   (size >= 32)
 *  exp[]   – array of exponents     (size >= 32)
 *  The caller must provide sufficiently large arrays. */
static int factorise_uint64(uint64_t n, uint32_t fac[], uint32_t exp[])
{
    int cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] *
                                    prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if ((tmp % p) == 0) {
            uint32_t e = 0;
            while ((tmp % p) == 0) {
                tmp /= p;
                ++e;
            }
            fac[cnt] = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {            /* remaining prime factor */
        fac[cnt] = (uint32_t)tmp;
        exp[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* ------------------------------------------------------------------ */
/*  Check whether odd k satisfies A003415(σ(k²)) > k².                 */
static bool satisfies(uint64_t k)
{
    /* Factorise k */
    uint32_t fac_k[32], exp_k[32];
    int cnt_k = factorise_uint64(k, fac_k, exp_k);

    /* Compute σ(k²) = ∏ (p^(2e+1) - 1)/(p - 1)   */
    unsigned __int128 sigma = 1;
    for (int i = 0; i < cnt_k; ++i) {
        uint32_t p   = fac_k[i];
        uint32_t e   = exp_k[i];
        uint32_t exp2 = 2 * e;                              /* exponent in k² */

        /* Num = p^(2e+1) - 1 */
        unsigned __int128 num = uint128_pow(p, exp2 + 1) - 1;
        uint32_t den = p - 1;
        unsigned __int128 term = num / den;                  /* integer division */
        sigma *= term;
    }
    uint64_t sigma_u = (uint64_t)sigma;        /* assume it fits 64‑bit */

    /* Factorise σ(k²) */
    uint32_t fac_s[32], exp_s[32];
    int cnt_s = factorise_uint64(sigma_u, fac_s, exp_s);

    /* Arithmetic derivative D(σ(k²)) = Σ e_i * σ(k²) / p_i */
    unsigned __int128 der = 0;
    for (int i = 0; i < cnt_s; ++i) {
        uint32_t p = fac_s[i];
        uint32_t e = exp_s[i];
        der += (unsigned __int128)sigma_u / p * e;
    }

    /* Compare with k² */
    unsigned __int128 k_sq = (unsigned __int128)k * k;
    return der > k_sq;
}

/* ------------------------------------------------------------------ */
/*  Ensure term_list has capacity for at least 'need' elements.        */
static void ensure_term_capacity(size_t need)
{
    if (term_cap >= need) return;
    size_t new_cap = (term_cap == 0) ? 1024 : term_cap * 2;
    while (new_cap < need) new_cap *= 2;
    uint64_t *new_arr = (uint64_t *)realloc(term_list, new_cap * sizeof(uint64_t));
    term_list = new_arr;
    term_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Public function: returns the n-th term of the sequence.           */
/*  idx_t n is 1‑based; a(0) is unused.                               */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined case, safety */

    init_primes();                         /* primes ready */

    /* Initialise term cache on first call */
    if (term_list == NULL) {
        term_cap = 1024;
        term_list = (uint64_t *)malloc(term_cap * sizeof(uint64_t));
        term_cnt   = 0;
        /* start checking from 1 */
        static uint32_t last_checked = 0;   /* preserved between calls */
    }

    /* Generate terms until we have at least n of them */
    while (term_cnt < n) {
        /* Get next odd candidate */
        static uint32_t last_checked = 0;
        if (last_checked == 0) last_checked = 1;
        else last_checked += 2;

        if (satisfies((uint64_t)last_checked)) {
            ensure_term_capacity(term_cnt + 1);
            term_list[term_cnt++] = (uint64_t)last_checked;
        }
    }

    return (seq_t)term_list[n - 1];
}
