/*
 * A317091  Positive integers whose prime multiplicities are weakly increasing
 *          and span an initial interval of positive integers.
 * https://oeis.org/A317091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A317091
    positive integers whose prime exponents are weakly increasing and
    span an initial interval of positive integers
    (exponents 1,2,…,t all occur for some t)
    The function computes the n‑th term by testing integers in order,
    factorising them, and storing the values satisfying the property. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PRIMELIMIT 2000000   /* upper bound for primes needed */

static uint32_t *prime_list = NULL;
static size_t     prime_cnt   = 0;

/* Initialise sieve and prime list once */
static void init_primes(void)
{
    if (prime_cnt) return;  /* already initialised */

    bool *sieve = (bool *)calloc(PRIMELIMIT + 1, 1);
    for (uint32_t i = 2;
         (uint64_t)i * i <= PRIMELIMIT;
         ++i)
        if (!sieve[i])
            for (uint32_t j = i * i; j <= PRIMELIMIT; j += i)
                sieve[j] = true;

    /* count primes */
    for (uint32_t i = 2; i <= PRIMELIMIT; ++i)
        if (!sieve[i]) ++prime_cnt;

    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    size_t pos = 0;
    for (uint32_t i = 2; i <= PRIMELIMIT; ++i)
        if (!sieve[i]) prime_list[pos++] = i;

    free(sieve);
}

/* Test whether v satisfies the required exponent property. */
static bool is_valid_number(seq_t v)
{
    if (v < (seq_t)2) return false;

    seq_t tmp = v;
    int  prev_exp = -1;
    bool seen[128] = {0};
    int  max_exp   = 0;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        seq_t p = (seq_t)prime_list[i];
        /* if p*p > tmp break */
        if (p * p > tmp) break;

        if (tmp % p == 0)
        {
            int exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            if (exp < prev_exp) return false;
            prev_exp = exp;
            seen[exp] = true;
            if (exp > max_exp) max_exp = exp;
        }
    }

    /* remaining factor > 1 is prime with exponent 1 */
    if (tmp > (seq_t)1)
    {
        int exp = 1;
        if (exp < prev_exp) return false;
        prev_exp = exp;
        seen[exp] = true;
        if (exp > max_exp) max_exp = exp;
    }

    if (!seen[1]) return false;                /* must contain exponent 1 */
    for (int e = 1; e <= max_exp; ++e)
        if (!seen[e]) return false;             /* all 1..t must appear */

    return true;
}

/* Ensure cache can store at least need elements */
static void grow_cache(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap == 0) ? 4 : *cap;
    while (newcap < need) newcap <<= 1;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* ---------------------------------------------------------------
   OEIS sequence A317091
   --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;
    static seq_t cur = (seq_t)2;   /* next integer to test */

    init_primes();

    if (n == 0) return (seq_t)0;   /* undefined, safeguard */

    /* generate terms until we have n of them */
    while (cache_len < (size_t)n)
    {
        if (is_valid_number(cur))
        {
            grow_cache(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = cur;
        }
        ++cur;
    }

    return cache[n - 1];
}
