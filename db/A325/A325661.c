/*
 * A325661  q-powerful numbers. Numbers whose factorization into factors
 *          prime(i)/i has no factor of multiplicity 1.
 * https://oeis.org/A325661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 847, u32 n <= 1071, u64 n <= 1071,
 *            u128 n <= 1071, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: q‑powerful numbers (OEIS A325661).  
   Computed by enumerating integers and checking that the
   exponents in their unique q‑factorization are never 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------
   Sieve for smallest prime factor (spf) up to LIMIT.
   The LIMIT is chosen large enough to cover the needed range
   (first 10,000 terms lie far below this bound).          ---- */
#define LIMIT (20000000)

static int *spf       = NULL;       /* spf[n] = smallest prime divisor of n */
static int *prime_tbl = NULL;       /* array of primes in increasing order */
static size_t prime_cnt = 0;        /* number of primes */

static void init_sieve(void)
{
    if (spf != NULL) return;   /* already initialized */

    spf = (int *)calloc(LIMIT + 1, sizeof(int));
    if (!spf) exit(EXIT_FAILURE);

    /* temporary array for primes: capacity ~ LIMIT / 10 is more than enough */
    prime_tbl = (int *)malloc(2000000 * sizeof(int));
    if (!prime_tbl) exit(EXIT_FAILURE);

    for (int i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            prime_tbl[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt && prime_tbl[j] <= spf[i];
             ++j) {
            long long v = (long long)i * prime_tbl[j];
            if (v > LIMIT) break;
            spf[(int)v] = prime_tbl[j];
        }
    }
}

/* Binary search: return 1‑based index of prime p in prime_tbl.      */
static int prime_index_of(int p)
{
    int lo = 0, hi = (int)prime_cnt - 1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        int q = prime_tbl[mid];
        if (q == p) return mid + 1;   /* 1‑based index */
        if (q < p)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return -1;  /* should never happen for prime p <= LIMIT */
}

/* --------------------------------------------------------------
   Check whether m (m <= LIMIT) is a q‑powerful number.
   The algorithm reconstructs the unique q‑factorization and
   verifies that no exponent equals 1.                            */
static bool is_q_powerful(unsigned long long m)
{
    if (m == 1) return true;

    /* For each distinct index i that occurs, we store its exponent
       in a small array (max 64 entries, enough for all numbers <= LIMIT). */
    struct {int idx; int exp;} exps[64];
    int exps_cnt = 0;

    unsigned long long cur = m;
    while (cur > 1) {
        int p = spf[(int)cur];                /* smallest prime factor */
        int idx = prime_index_of(p);          /* prime's index */
        if (idx < 1) return false;            /* should not happen */

        /* Update exponent for this index */
        int k;
        for (k = 0; k < exps_cnt; ++k)
            if (exps[k].idx == idx) { ++exps[k].exp; break; }
        if (k == exps_cnt) {
            if (exps_cnt >= 64) return false;   /* safety guard */
            exps[exps_cnt].idx = idx;
            exps[exps_cnt].exp = 1;
            ++exps_cnt;
        }
        /* Transform the number according to the factorisation rule.
           m = m * idx / p  (p divides m by construction)          */
        cur = (cur / (unsigned long long)p) * (unsigned long long)idx;
    }

    /* No exponent should be 1 */
    for (int i = 0; i < exps_cnt; ++i)
        if (exps[i].exp == 1) return false;

    return true;
}

/* --------------------------------------------------------------
   Cache of already generated q‑powerful numbers.  The array is
   expanded lazily as needed.                                        */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements stored */
static size_t cache_cap = 0;   /* allocated capacity */
static unsigned long long next_candidate = 1;   /* next integer to test */

/* Ensure that the cache contains at least n elements.             */
static void ensure_cache(idx_t n)
{
    init_sieve();        /* build sieve on first use */
    while (cache_len < n) {
        if (is_q_powerful(next_candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* --------------------------------------------------------------
   Public function required by the specification.                */
seq_t a(idx_t n)
{
    static bool init_once = false;
    if (!init_once) { init_once = true; }   /* trigger static init */
    ensure_cache(n);
    return cache[n - 1];
}
