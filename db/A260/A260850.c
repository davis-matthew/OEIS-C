/*
 * A260850  Lexicographically earliest sequence such that for any n>1, n=u*v,
 *          where u/v = a(n)/a(n-1) in reduced form.
 * https://oeis.org/A260850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * ∏ p_i^{v_i*e_i} where v_i = +1 if e_i >
 *            v_{p_i}(a(n-1)), * v_i = -1 otherwise. Offset: 1, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 31, u64 n <= 66, u128 n <= 127,
 *            big any n
 * Helpers:   grow_seq, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"


/* ----------  State for memoisation and auxiliary data --------------- */
static seq_t *seq_cache = NULL;      /* array of computed terms  */
static size_t cache_len = 0;         /* number of terms stored       */
static size_t cache_cap = 0;         /* current capacity              */

static uint64_t *spf = NULL;         /* smallest prime factor array   */
static idx_t spf_cap = 0;            /* largest n for which spf is valid */

static uint64_t *prime_list = NULL;  /* list of primes up to spf_cap */
static size_t primes_cnt = 0;        /* number of primes */
static int *prime_exps = NULL;       /* exponent of each prime in the current a(n) */

/* ----------  Helper functions  ------------------------------------- */

/* Binary search to find the index of prime p in prime_list.  Assumes p is prime. */
static int find_prime_index(uint64_t p)
{
    int lo = 0, hi = (int)primes_cnt - 1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return mid;
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;            /* should never happen */
}

/* Build spf array and prime list up to at least n.  The arrays are grown
   as necessary.  All newly added prime_exps entries are initialised to 0. */
static void build_spf_and_primes(idx_t n)
{
    if (n <= spf_cap) return;          /* already computed */

    /* grow spf array */
    uint64_t *new_spf = (uint64_t *)realloc(spf, sizeof(uint64_t) * (n + 1));
    if (!new_spf) abort();
    spf = new_spf;
    for (idx_t i = 2; i <= n; ++i) spf[i] = 0;

    /* simple sieve for smallest prime factor */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_cap = n;

    /* count primes up to n */
    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (spf[i] == i) ++cnt;

    /* grow prime list and prime_exps array */
    if (cnt > primes_cnt) {
        uint64_t *new_prime_list = (uint64_t *)realloc(prime_list, sizeof(uint64_t) * cnt);
        int *new_prime_exps = (int *)realloc(prime_exps, sizeof(int) * cnt);
        if (!new_prime_list || !new_prime_exps) abort();
        prime_list = new_prime_list;
        prime_exps = new_prime_exps;
        for (size_t i = primes_cnt; i < cnt; ++i) prime_exps[i] = 0;
        size_t idx = 0;
        for (idx_t i = 2; i <= n; ++i)
            if (spf[i] == i) prime_list[idx++] = i;
        primes_cnt = cnt;
    }
}

/* Compute the current a(n) from the prime_exps array.  Works for both
   integer and arbitrary‑precision seq_t, because it uses seq_mul and seq_pow. */
static seq_t compute_value_from_exponents(void)
{
    seq_t val = 1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        int e = prime_exps[i];
        if (e == 0) continue;
        uint64_t p = prime_list[i];
        seq_t mult = seq_pow((seq_t)p, e);
        val = seq_mul(val, mult);
    }
    return val;
}

/* Ensure that seq_cache contains all terms up to and including index n. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return; /* undefined below offset */

    /* create or enlarge required auxiliary data */
    build_spf_and_primes(n);
    if (cache_cap < (size_t)n)
        grow_seq(&seq_cache, &cache_cap, (size_t)n);

    /* initialise first term if needed */
    if (cache_len == 0) {
        seq_cache[0] = (seq_t)1;          /* a(1) = 1 */
        cache_len = 1;
    }

    /* compute missing terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        idx_t temp = i;
        /* factorise i using spf */
        while (temp > 1) {
            uint64_t p = spf[temp];
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            int pi = find_prime_index(p);
            if (pi < 0) abort();          /* should not happen */

            if (e > (idx_t)prime_exps[pi])
                prime_exps[pi] += (int)e;   /* multiply case */
            else
                prime_exps[pi] -= (int)e;   /* divide case */
        }
        /* after updating exponents, recompute the value from scratch */
        seq_cache[i - 1] = compute_value_from_exponents();
    }
    cache_len = (size_t)n;
}

/* ----------  Public function required by the build ------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* below offset, undefined */
    ensure_cache(n);
    return seq_cache[n - 1];
}
