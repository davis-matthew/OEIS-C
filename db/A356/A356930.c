/*
 * A356930  Numbers whose prime indices have all odd prime indices. MM-numbers
 *          of finite multisets of finite multisets of odd numbers.
 * https://oeis.org/A356930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A356930: Numbers whose prime indices have all odd prime indices.
   Implementation: precompute prime table and smallest prime factors up to a
   fixed limit.  For every number we test whether all of its prime factors p
   have an index idx(p) such that every prime factor of idx(p) has an odd
   index itself.  The sequence terms are generated in ascending order and
   cached for random access. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* typenames supplied by the build system */
 /* seq_t and idx_t are already defined */

static const idx_t LIMIT = 2000000;                      /* sieve limit     */
static idx_t *spf          = NULL;                       /* smallest prime factor   */
static idx_t *prime_index  = NULL;                       /* prime -> 1‑based index  */
static unsigned char *good_idx = NULL;                    /* good_idx[i] = 1 if all prime indices of i are odd */
static seq_t *cache = NULL;                               /* generated sequence      */
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static bool initialized = false;

/* ---------- utilities ---------- */

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ---------- initialization: sieve, spf, prime_index, good_idx ---------- */

static void init(void)
{
    if (initialized)
        return;
    initialized = true;

    /* allocate arrays */
    spf        = (idx_t*)malloc((LIMIT + 1) * sizeof(idx_t));
    prime_index = (idx_t*)malloc((LIMIT + 1) * sizeof(idx_t));
    good_idx   = (unsigned char*)calloc((LIMIT + 1), sizeof(unsigned char));

    /* linear sieve */
    idx_t *primes = (idx_t*)malloc((LIMIT / 10 + 1) * sizeof(idx_t));
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            primes[primes_cnt] = i;
            prime_index[i] = ++primes_cnt;   /* 1‑based index */
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            if (p > spf[i] || i * p > LIMIT)
                break;
            spf[i * p] = p;
        }
    }
    free(primes);

    /* pre‑compute good_idx for 1 .. number of primes <= LIMIT */
    good_idx[1] = 1;                     /* 1 has no prime indices   */
    /* number of primes is primes_cnt, but we only use indices up to that */
    for (size_t i = 2; i <= (size_t)primes_cnt; ++i) {
        idx_t x = i;
        unsigned char ok = 1;
        while (x > 1) {
            idx_t q = spf[x];                 /* prime factor of i   */
            idx_t idxq = prime_index[q];      /* index of that prime */
            if ((idxq & 1) == 0) {            /* even index – fail   */
                ok = 0;
                break;
            }
            while (x % q == 0)
                x /= q;
        }
        good_idx[i] = ok;
    }
}

/* ---------- test if a candidate satisfies the property ---------- */

static bool good_candidate(idx_t n)
{
    if (n == 1)
        return true;
    idx_t x = n;
    while (x > 1) {
        idx_t p = spf[x];                /* prime factor of n   */
        idx_t idxp = prime_index[p];     /* index of that prime */
        if (!good_idx[idxp])
            return false;
        while (x % p == 0)
            x /= p;
    }
    return true;
}

/* ---------- extend cache to contain at least 'target' terms ---------- */

static void extend_to(idx_t target)
{
    static idx_t last_checked = 1;          /* last candidate checked */
    while (cache_len < target) {
        if (good_candidate(last_checked)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
        ++last_checked;
    }
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    init();
    if (n == 0)
        return (seq_t)0;                     /* undefined for 0 */
    extend_to(n);
    return cache[n - 1];
}
