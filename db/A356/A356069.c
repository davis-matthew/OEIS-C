/*
 * A356069  Number of divisors of n whose prime indices cover an interval of
 *          positive integers (A073491).
 * https://oeis.org/A356069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that the set of prime
 *            indices of d forms a contiguous interval of positive integers
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Minimal sieve – enough for primes up to 20000 (covers all primes
   needed for n ≤ 10000).  The sieve is performed once. */
static bool primes_initialized = false;
static int prime_count = 0;
static int prime_index[20001] = {0};
static int prime_list[20000];

static void init_primes(void)
{
    if (primes_initialized)
        return;

    const idx_t LIMIT = 20000;
    unsigned char *is_comp = (unsigned char*)malloc(LIMIT + 1);
    for (idx_t i = 0; i <= LIMIT; ++i)
        is_comp[i] = 0;
    for (idx_t i = 2; i * i <= LIMIT; ++i)
    {
        if (!is_comp[i])
        {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;
        }
    }
    prime_count = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
    {
        if (!is_comp[i])
        {
            prime_index[i] = ++prime_count;      /* 1‑based index */
            prime_list[prime_count - 1] = (int)i;
        }
    }
    free(is_comp);
    primes_initialized = true;
}

/* Recursive enumeration of all combinations of exponents. */
static void dfs_divisors(int idx,
                         int fact_cnt,
                         int cur_distinct,
                         int cur_min,
                         int cur_max,
                         const int *prime_idx,
                         const int *exps,
                         seq_t *cnt)
{
    if (idx == fact_cnt)
    {
        if (cur_distinct == 0 || (cur_max - cur_min + 1 == cur_distinct))
            ++(*cnt);
        return;
    }

    /* Take exponent 0 for this prime */
    dfs_divisors(idx + 1, fact_cnt, cur_distinct, cur_min, cur_max,
                 prime_idx, exps, cnt);

    /* Take exponents ≥1; each distinct exponent gives a different divisor. */
    int pi = prime_idx[idx];
    int new_min = (cur_distinct == 0) ? pi : ((cur_min < pi) ? cur_min : pi);
    int new_max = (cur_distinct == 0) ? pi : ((cur_max > pi) ? cur_max : pi);
    int new_dist = cur_distinct + 1;

    for (int e = 1; e <= exps[idx]; ++e)
    {
        dfs_divisors(idx + 1, fact_cnt, new_dist, new_min, new_max,
                     prime_idx, exps, cnt);
    }
}

seq_t a(idx_t n)
{
    init_primes();

    /* Factorize n. */
    idx_t rem = n;
    const int MAX_FACT = 20;
    int factors[MAX_FACT];
    int exps[MAX_FACT];
    int fact_cnt = 0;

    for (int i = 0; i < prime_count &&
         (idx_t)prime_list[i] * (idx_t)prime_list[i] <= rem; ++i)
    {
        int p = prime_list[i];
        if (rem % p == 0)
        {
            int e = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++e;
            }
            factors[fact_cnt] = p;
            exps[fact_cnt] = e;
            ++fact_cnt;
        }
    }
    if (rem > 1)
    {
        factors[fact_cnt] = (int)rem;
        exps[fact_cnt] = 1;
        ++fact_cnt;
    }

    /* Prepare prime indices of the factors. */
    int prime_idx[MAX_FACT];
    for (int i = 0; i < fact_cnt; ++i)
        prime_idx[i] = prime_index[factors[i]];  /* 1‑based */

    seq_t result = 0;
    dfs_divisors(0, fact_cnt, 0, 0, 0, prime_idx, exps, &result);
    return result;
}
