/*
 * A324935  Matula-Goebel numbers of rooted trees whose non-leaf terminal
 *          subtrees are all different.
 * https://oeis.org/A324935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes terms of A324935 by generating all positive integers
 *            whose unique Matula–Goebel factorization uses each q(i) (i>1) at
 *            most once
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* --------------------------------------------------------------- */
/* Globals for prime sieve and term cache                          */
/* --------------------------------------------------------------- */

#define SPF_LIMIT 3000000            /* upper bound for needed primes */

/* Sieving data */
static uint32_t *spf          = NULL;   /* smallest prime factor */
static uint32_t *primes       = NULL;   /* list of primes */
static size_t    primes_cnt   = 0;
static uint32_t *prime_index  = NULL;   /* mapping prime → index */

/* Count of occurrences of each index i>1 in current factorisation */
static unsigned char *idx_counts = NULL;

/* Cache of sequence values */
static seq_t *term_cache   = NULL;
static size_t term_size    = 0;
static size_t term_cap     = 0;
static idx_t last_checked  = 0;

/* --------------------------------------------------------------- */
/* Helper: initialise sieve and associated tables                */
/* --------------------------------------------------------------- */

static void init_factorisation(void)
{
    if (spf) return;  /* already initialised */

    /* 1. Allocate and initialise spf array */
    spf = (uint32_t*)malloc((SPF_LIMIT + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= SPF_LIMIT; ++i) spf[i] = (uint32_t)i;

    for (size_t i = 2; i * i <= SPF_LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j <= SPF_LIMIT; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }

    /* 2. Collect primes */
    primes = (uint32_t*)malloc((SPF_LIMIT + 1) * sizeof(uint32_t)); /* over‑alloc */
    for (size_t i = 2; i <= SPF_LIMIT; ++i)
        if (spf[i] == (uint32_t)i) primes[primes_cnt++] = (uint32_t)i;

    /* 3. Map prime value → index (starting at 1) */
    prime_index = (uint32_t*)calloc(SPF_LIMIT + 1, sizeof(uint32_t));
    for (size_t i = 0; i < primes_cnt; ++i)
        prime_index[primes[i]] = (uint32_t)(i + 1);

    /* 4. Counter array for indices */
    idx_counts = (unsigned char*)calloc(primes_cnt + 2, sizeof(unsigned char));

    /* 5. Initialise cache */
    term_cap   = 65536;
    term_cache = (seq_t*)malloc(term_cap * sizeof(seq_t));
    term_size  = 0;
    last_checked = 0;
}

/* --------------------------------------------------------------- */
/* Helper: test whether a given integer (<= SPF_LIMIT) belongs   */
/* --------------------------------------------------------------- */

static bool in_sequence(idx_t value)
{
    if (value > SPF_LIMIT) return false;   /* outside pre‑computed range */

    uint32_t n = (uint32_t)value;

    /* strip all factors of 2 – they correspond to q(1) which may repeat */
    while ((n & 1u) == 0u) n >>= 1u;

    /* visited indices for this factorisation, used to reset counts */
    int visited[64];
    int visited_len = 0;

    while (n > 1u) {
        /* find the largest odd prime divisor of n */
        uint32_t temp = n;
        uint32_t maxp = 0u;
        while (temp > 1u) {
            uint32_t p = spf[temp];
            maxp = p;
            while (temp % p == 0u) temp /= p;
        }

        uint32_t idx = prime_index[maxp];      /* idx > 1 since maxp is odd */
        if (++idx_counts[idx] > 1u) {          /* repeated usage of q(i), i>1 */
            for (int i = 0; i < visited_len; ++i) idx_counts[visited[i]] = 0;
            return false;
        }
        visited[visited_len++] = (int)idx;

        /* transform n :  n = n / maxp * idx  (guaranteed integer) */
        n = (n / maxp) * idx;

        /* remove any new powers of two produced by the multiplication */
        while ((n & 1u) == 0u) n >>= 1u;
    }

    /* reset counters for indices used in this factorisation */
    for (int i = 0; i < visited_len; ++i) idx_counts[visited[i]] = 0;
    return true;
}

/* --------------------------------------------------------------- */
/* Helper: grow the cache until it contains at least target terms */
/* --------------------------------------------------------------- */

static void extend_cache(idx_t target)
{
    while ((size_t)term_size < (size_t)target) {
        ++last_checked;
        if (in_sequence(last_checked)) {
            if (term_size == term_cap) {
                term_cap <<= 1;
                term_cache = (seq_t*)realloc(term_cache,
                                              term_cap * sizeof(seq_t));
            }
            term_cache[term_size++] = (seq_t)last_checked;
        }
    }
}

/* --------------------------------------------------------------- */
/* Sequence function: public interface                              */
/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, return 0 */

    init_factorisation();          /* ensures tables are ready */
    extend_cache(n);               /* make sure we have at least n terms */

    return term_cache[n - 1];
}
