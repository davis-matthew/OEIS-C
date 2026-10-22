/*
 * A377563  Numbers that have fewer infinitary divisors than noninfinitary
 *          divisors.
 * https://oeis.org/A377563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 4770, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A377563: Numbers that have fewer infinitary divisors than noninfinitary divisors.
   For n ≥ 1, a(n) is the n‑th such integer, with a(1)=16, a(2)=36, … */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---- sieve for primes up to MAX_SIEVE ---- */
#define MAX_SIEVE 1000000   /* enough for terms up to 10000 */
static bool sieve_init = false;
static int  primes[MAX_SIEVE + 1];
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (sieve_init) return;
    memset(primes, 0, sizeof(primes));
    for (int i = 2; i <= MAX_SIEVE; ++i) {
        if (primes[i] == 0) {
            primes[primes_cnt++] = i;
            if ((int64_t)i * i <= MAX_SIEVE) {
                for (int j = i * i; j <= MAX_SIEVE; j += i)
                    primes[j] = 1;
            }
        }
    }
    sieve_init = true;
}

/* ---- factorisation of an integer <= MAX_SIEVE ---- */
static void factor_by_primes(uint64_t n, uint64_t *exp_arr, size_t *cnt)
{
    *cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            uint64_t e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                ++e;
            }
            exp_arr[(*cnt)++] = e;
        }
    }
    if (tmp > 1)  /* remaining prime factor */
        exp_arr[(*cnt)++] = 1;
}

/* ---- test if integer x satisfies the condition ---- */
static bool satisfies(uint64_t x)
{
    if (x < 2) return false;      /* 1 is not accepted */
    uint64_t exp_arr[64];
    size_t   cnt;
    factor_by_primes(x, exp_arr, &cnt);

    uint64_t d_inf = 1;   /* number of infinitary divisors */
    uint64_t d_tot = 1;   /* total number of (non‑infinitary) divisors */

    for (size_t i = 0; i < cnt; ++i) {
        unsigned long long pop = __builtin_popcountll(exp_arr[i]); /* popcount of exponent */
        d_inf <<= pop;                     /* multiply by 2^pop */
        d_tot *= (exp_arr[i] + 1ULL);
    }
    return (d_inf << 1ULL) < d_tot;         /* 2*d_inf < d_tot */
}

/* ---- dynamic cache for computed terms ---- */
static seq_t *cache   = NULL;   /* array of seq_t values */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t  cache_len = 0;     /* number of stored terms */
static uint64_t checked_up_to = 0;  /* highest integer examined */

/* ---- function required by the framework ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range */

    /* initialise primes once */
    init_primes();

    /* extend cache until it contains at least n terms */
    while (cache_len < n) {
        ++checked_up_to;
        if (satisfies(checked_up_to)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)checked_up_to;
        }
    }
    return cache[n - 1];
}
