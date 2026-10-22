/*
 * A301975  Numbers whose abundance is divisible by its number of divisors.
 * https://oeis.org/A301975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A301975 – Numbers whose abundance is divisible by the number of
 * divisors.
 *
 * a(n) = the n-th integer k such that
 *        (sigma(k) - 2*k) is divisible by d(k),
 * where sigma(k) is the sum of all positive divisors of k
 * and d(k) is the number of divisors of k.
 *
 * Above a recursive definition this sequence is obtained by
 * searching integers in increasing order.  The implementation
 * keeps a dynamic array of found terms and a sieve of smallest
 * prime factors to factor each candidate efficiently.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"

/* Static cache of the sequence terms */
static idx_t   last_checked = 0;          /* last integer examined      */
static seq_t * terms        = NULL;       /* found terms                */
static size_t term_cap     = 0;           /* capacity of terms array    */
static size_t term_len     = 0;           /* number of terms stored     */

/* Smallest prime factor array */
static idx_t * spf          = NULL;       /* spf[i] = smallest prime factor of i, i>=2 */
static idx_t   spf_limit    = 0;          /* maximum index covered by spf          */

/* Ensure that spf array covers at least the given limit */
static void ensure_spf(idx_t limit)
{
    if (spf_limit >= limit)
        return;
    if (spf != NULL)
        free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Append a new term to the dynamic terms array */
static void append_term(seq_t val)
{
    if (term_len >= term_cap) {
        size_t new_cap = (term_cap == 0) ? 16 : term_cap * 2;
        /* cast explicitely for C++ compatibility */
        seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr)
            abort();
        terms = new_arr;
        term_cap = new_cap;
    }
    terms[term_len++] = val;
}

/* Multiply two 64‑bit integers modulo mod using a 128‑bit intermediate
 * to avoid overflow.  Mod is always > 0. */
static inline unsigned long long mul_mod64(unsigned long long a,
                                           unsigned long long b,
                                           unsigned long long mod)
{
    return (unsigned long long)((unsigned __int128)a * b % mod);
}

/* Examine the next integer, test for membership, and store if appropriate */
static void examine_next(void)
{
    idx_t n = ++last_checked;             /* next integer to test      */

    /* Factor n using SPF to collect prime exponents */
    const int MAX_FACTORS = 16;           /* safe upper bound */
    uint64_t primes[MAX_FACTORS];
    int       exps[MAX_FACTORS];
    int       num_factors = 0;
    uint64_t  temp = (uint64_t)n;

    while (temp > 1 && num_factors < MAX_FACTORS) {
        uint64_t p = (uint64_t)spf[temp];
        uint64_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        primes[num_factors] = p;
        exps[num_factors]   = (int)e;
        ++num_factors;
    }

    /* Compute the number of divisors d = Π (e+1) */
    uint64_t d = 1;
    for (int i = 0; i < num_factors; ++i)
        d *= (uint64_t)(exps[i] + 1);

    /* Compute sigma(n) modulo d using multiplicativity */
    uint64_t sigma_mod = 1;   /* current sigma(n) mod d */
    for (int i = 0; i < num_factors; ++i) {
        uint64_t p   = primes[i];
        int      e   = exps[i];
        uint64_t pow = 1;   /* p^0 mod d */
        uint64_t sum = 1;   /* Σ_{k=0..e} p^k mod d */

        for (int k = 1; k <= e; ++k) {
            pow = mul_mod64(pow, p, d);
            sum = (sum + pow) % d;
        }
        sigma_mod = mul_mod64(sigma_mod, sum, d);
    }
    sigma_mod %= d;           /* final sigma(n) mod d */

    /* Compute (2*n) mod d */
    uint64_t mod_two_n = ((uint64_t)(n % d) * 2ULL) % d;

    if (sigma_mod == mod_two_n)
        append_term((seq_t)n);
}

/* Ensure the cache contains at least 'need' terms */
static void ensure_cache(idx_t need)
{
    while (term_len < need) {
        /* Grow the SPF array if needed for the next number */
        if (last_checked >= spf_limit) {
            idx_t new_limit = (spf_limit == 0) ? 1024 : spf_limit * 2;
            while (last_checked >= new_limit)
                new_limit *= 2;
            ensure_spf(new_limit);
        }
        examine_next();
    }
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, but safe for safety */
    ensure_cache(n);
    return terms[n - 1];
}
