/*
 * A368325  Union of primorials and powers of 2.
 * https://oeis.org/A368325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Union of A000079 and A002110 powers of two (with 2^0 = 1)
 *            and primorials (product of the first k primes)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 40, u64 n <= 78, u128 n <= 153,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* global state for caching and sequence generation */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

static seq_t power_val   = 1;   /* current power of 2 (2^k) */
static bool   power_done = false;   /* true if power overflowed seq_t */

static seq_t prim_val    = 2;   /* current primorial (product of first m primes) */
static size_t prim_idx   = 0;   /* index of the last prime used in prim_val */
static bool   prim_done  = false;   /* true if primorial overflowed seq_t */

static uint64_t *primes    = NULL; /* array of precomputed small primes */
static size_t   primes_cnt = 0;     /* number of primes in the array */

/* ------------------------------------------------------------------ */
/* helper: allocate or grow the cache array                          */
static void ensure_cache_space(void)
{
    if (cache_len < cache_cap)
        return;
    cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* ------------------------------------------------------------------ */
/* helper: simple sieve up to a fixed limit to obtain small primes   */
static void generate_primes(void)
{
    const idx_t limit = 200000;                /* enough for all required primorials */
    char *is_comp = (char *)calloc(limit + 1, 1);
    if (!is_comp) return; /* cannot happen for such small limit */

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;

    /* allocate storage for primes (upper bound */
    primes = (uint64_t *)malloc((limit / 10) * sizeof(uint64_t));
    if (!primes) { free(is_comp); return; }

    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) {
            if (primes_cnt == 0)
                primes_cnt = 0;
            primes[primes_cnt++] = (uint64_t)i;
        }
    free(is_comp);
}

/* ------------------------------------------------------------------ */
/* initialization helper                                               */
static void init_seq(void)
{
    if (cache) return;          /* already initialized */

    /* initialise cache */
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_len = 0;

    /* prime generation */
    generate_primes();

    /* initialise sequences */
    power_val   = 1;
    power_done  = false;

    prim_val    = 2;
    prim_idx    = 0;
    prim_done   = false;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                              */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined offset; safe fallback */

    init_seq();

    while (cache_len < (size_t)n)
    {
        seq_t cand_power = power_done ? ((seq_t)-1) : power_val;
        seq_t cand_prim  = prim_done  ? ((seq_t)-1) : prim_val;
        seq_t next = (cand_power < cand_prim) ? cand_power : cand_prim;

        /* store the chosen value */
        ensure_cache_space();
        cache[cache_len++] = next;

        /* advance power sequence if it was used */
        if (!power_done && power_val <= cand_prim)
        {
            seq_t new_power = power_val * (seq_t)2;
            if (new_power < power_val)       /* overflow detected */
                power_done = true;
            else
                power_val = new_power;
        }

        /* advance primorial sequence if it was used */
        if (!prim_done && prim_val <= cand_power)
        {
            if (prim_idx + 1 < primes_cnt)
            {
                seq_t new_prim = prim_val * (seq_t)primes[prim_idx + 1];
                if (new_prim < prim_val)   /* overflow detected */
                    prim_done = true;
                else
                {
                    prim_val = new_prim;
                    prim_idx++;
                }
            }
            else
                prim_done = true;
        }
    }

    return cache[(size_t)n - 1];
}
