/*
 * A383089  Numbers whose prime indices have more than one permutation with
 *          all equal run-lengths.
 * https://oeis.org/A383089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th integer whose prime‑factor indices multiset
 * has at least two permutations with all equal run‑lengths.
 * The code generates the sequence lazily: it keeps a cache of already
 * found terms and tests successive integers until the cache contains
 * n entries.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime sieve ------------------------------------------------------- */
#define PRIME_SIEVE_LIMIT 1000000   /* sufficient for < 10⁶ candidates */

static uint32_t prime_list[PRIME_SIEVE_LIMIT];
static size_t prime_count = 0;

static void init_prime_list(void)
{
    static bool inited = false;
    if (inited) return;
    inited = true;
    static unsigned char is_comp[PRIME_SIEVE_LIMIT + 1] = {0};
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
    {
        if (!is_comp[i])
        {
            prime_list[prime_count++] = i;
            if ((uint64_t)i * i <= PRIME_SIEVE_LIMIT)
            {
                for (uint64_t j = (uint64_t)i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Helper: gcd for unsigned int ------------------------------------- */
static unsigned int gcd_uint(unsigned int a, unsigned int b)
{
    while (b != 0)
    {
        unsigned int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Helper: dynamic array for cache ---------------------------------- */
static void ensure_capacity(size_t *cap, seq_t **p, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap == 0) ? 64 : *cap * 2;
    while (newcap < need) newcap <<= 1;
    *p = (seq_t*)realloc(*p, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* ------------------------------------------------------------------ */
/* Criterion test --------------------------------------------------- */
static bool qualifies(uint64_t n)
{
    /* factor n */
    uint64_t temp = n;
    unsigned int exps[64];
    size_t ecnt = 0;

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i]*prime_list[i] <= temp; ++i)
    {
        uint32_t p = prime_list[i];
        if (temp % p == 0)
        {
            unsigned int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            exps[ecnt++] = e;
        }
    }
    if (temp > 1) /* remaining prime factor */
    {
        exps[ecnt++] = 1;
    }

    /* if 0 or 1 distinct primes --> not in sequence */
    if (ecnt <= 1) return false;

    /* gcd of exponents */
    unsigned int g = exps[0];
    for (size_t i = 1; i < ecnt; ++i) g = gcd_uint(g, exps[i]);

    /* case 1 : common divisor >= 2 */
    if (g >= 2) return true;   /* more than one prime factor guarantees multiple permutations */

    /* case 2 : gcd == 1, need to analyse L = 1 case */
    unsigned int N = 0;            /* total number of prime factors (with multiplicity) */
    unsigned int maxe = exps[0];
    unsigned int mine = exps[0];
    for (size_t i = 0; i < ecnt; ++i)
    {
        N += exps[i];
        if (exps[i] > maxe) maxe = exps[i];
        if (exps[i] < mine) mine = exps[i];
    }

    /* no permutation can avoid adjacency */
    if (maxe > (N + 1) / 2) return false;

    /* uniqueness test for k == 2 */
    if (ecnt == 2)
    {
        if (maxe == mine + 1) return false;  /* only one valid arrangement */
        return true;
    }

    /* k >= 3 : at least two valid permutations exist */
    return true;
}

/* ------------------------------------------------------------------ */
/* Cache and main exported function --------------------------------- */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static uint64_t current = 1;  /* last integer tested */

static void ensure_cache(idx_t n)
{
    init_prime_list();
    while (seq_len < n)
    {
        ++current;
        if (qualifies(current))
        {
            ensure_capacity(&seq_cap, &seq_cache, seq_len + 1);
            seq_cache[seq_len++] = (seq_t)current;
        }
    }
}

seq_t a(idx_t n)
{
    if (seq_cache == NULL) { seq_cache = NULL; seq_len = 0; seq_cap = 0; }
    ensure_cache(n);
    return seq_cache[n - 1];
}
