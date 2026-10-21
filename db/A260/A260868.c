/*
 * A260868  Numbers for which the least k > 1 which divides n + 2^k - 2 is
 *          different from the smallest prime factor of n.
 * https://oeis.org/A260868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   least_divisor_expr, pow2_mod_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260868: Numbers for which the least k > 1 which divides
 * n + 2^k – 2 is different from the smallest prime factor of n.
 *
 * a(n) returns the nth term (1‑indexed).  The implementation uses a
 * linear sieve for the smallest prime factors and tests candidates
 * sequentially.  The cache allows calls in any order.  The code
 * works for all provided build types (uint8_t … uint64_t or arbitrary
 * precision) and runs comfortably under 2 s for n up to about 10 000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/least_divisor_expr.h"
#include "../utils/pow2_mod_k.h"

/* ------------------------------------------------------------ */
/*  Parameters (change if you need more terms)                  */
/* ------------------------------------------------------------ */
#ifndef SIEVE_LIMIT
# define SIEVE_LIMIT 2000000    /* should cover all terms needed for n≈10000 */
#endif

/* ------------------------------------------------------------ */
/*  Smallest‑prime‑factor (spf) table and prime list           */
/* ------------------------------------------------------------ */
static uint32_t *spf        = NULL;   /* size SIEVE_LIMIT+1, spf[0]=0, spf[1]=0    */
static uint32_t *primes     = NULL;   /* list of primes up to SIEVE_LIMIT          */
static size_t   primes_cnt  = 0;      /* number of primes stored                 */
static bool     spf_ready   = false;  /* flag: sieve already built                */

/* ------------------------------------------------------------ */
/*  Build a linear sieve up to SIEVE_LIMIT                     */
/* ------------------------------------------------------------ */
static void init_sieve(void)
{
    if (spf_ready) return;

    uint32_t N = SIEVE_LIMIT;
    spf = (uint32_t *)calloc((size_t)(N + 1), sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);

    /* Allocate space for primes */
    size_t prime_est = N / 10 + 10;        /* crude estimate */
    primes = (uint32_t *)malloc(prime_est * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    for (uint32_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_cnt < prime_est) primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt && primes[j] <= spf[i] &&
             (uint64_t)i * primes[j] <= N; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
    spf_ready = true;
}

/* ------------------------------------------------------------ */
/*  Smallest prime factor via SPF table or trial division      */
/* ------------------------------------------------------------ */
static uint32_t lpf_of(uint64_t n)
{
    if (n <= (uint64_t)SIEVE_LIMIT) return spf[(size_t)n];
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        if (n % primes[i] == 0) return primes[i];
    }
    return (uint32_t)n;   /* n itself is prime */
}

/* ------------------------------------------------------------ */
/*  2^e mod m (using 128‑bit multiplication to avoid overflow) */

/* ------------------------------------------------------------ */
/*  Least k > 1 that divides n + 2^k – 2                       */

/* ------------------------------------------------------------ */
/*  Cache for sequence terms                                   */
/* ------------------------------------------------------------ */
static seq_t *cache          = NULL;   /* array of terms */
static size_t cache_len      = 0;      /* number of valid terms */
static size_t cache_cap      = 0;      /* allocated capacity */
static uint64_t next_candidate = 2;    /* next integer to test (>=2) */

/* ------------------------------------------------------------ */
/*  Ensure we have enough capacity for the cache               */
/* ------------------------------------------------------------ */
static void ensure_cache_cap(size_t new_len)
{
    if (new_len <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 128;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  Public function: a(idx_t n)                                 */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but return 0 */
    init_sieve();          /* build sieve on first call */

    /* Generate terms until we have at least n entries */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate++;
        uint32_t lpf  = lpf_of(cand);
        uint32_t g    = least_divisor_expr(cand);
        if (g != lpf) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[(size_t)n - 1];
}
