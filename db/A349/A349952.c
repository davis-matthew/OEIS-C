/*
 * A349952  The smallest of 3 consecutive integers such that the first is
 *          divisible by the square of a prime, the second is divisible by the
 *          cube of a prime, and the third is divisible by the fourth power of
 *          a prime.
 * https://oeis.org/A349952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   build_primes_upto, has_any_prime_power, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A349952
   The smallest of 3 consecutive integers such that
   the first is divisible by the square of a prime,
   the second is divisible by the cube of a prime,
   the third is divisible by the fourth power of a prime.   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions provided by the build */
#include "../utils/pow_seq.h"           /* seq_t pow_seq(seq_t base, int exp); */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt); */
#include "../utils/has_any_prime_power.h"

/* ------------------------------------------------------------------ */
/* Primes – we need primes up to sqrt(maximum searched value).
   For n ≤ 10000 the required value is far below 2·10^7,
   so primes up to 50 000 cover all needed square, cube and
   4‑th‑power bases.  */
#define PRIME_UPPER_LIMIT 50000

static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;

/* prime power tables (one entry per prime) */
static seq_t   *sq_primes   = NULL;     /* p^2  */
static seq_t   *cube_primes = NULL;     /* p^3  */
static seq_t   *fourth_primes = NULL;   /* p^4  */
static size_t   primes_sq_cnt = 0;
static size_t   primes_cube_cnt = 0;
static size_t   primes_fourth_cnt = 0;

/* ------------------------------------------------------------------ */
/* Init primes and prime power tables once. */
static void init_primes_and_powers(void)
{
    if (prime_list != NULL) return;          /* already initialised */

    /* ---- primes --------------------------------------------------- */
    build_primes_upto((idx_t)PRIME_UPPER_LIMIT, &prime_list, &prime_count);

    /* ---- prime powers --------------------------------------------- */
    /* allocate maximal possible size (prime_count) */
    sq_primes   = (seq_t*)malloc(prime_count * sizeof(seq_t));
    cube_primes = (seq_t*)malloc(prime_count * sizeof(seq_t));
    fourth_primes = (seq_t*)malloc(prime_count * sizeof(seq_t));
    if (sq_primes == NULL || cube_primes == NULL || fourth_primes == NULL)
        exit(EXIT_FAILURE);

    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t p = prime_list[i];
        sq_primes[i]   = pow_seq((seq_t)p, 2);
        cube_primes[i] = pow_seq((seq_t)p, 3);
        fourth_primes[i] = pow_seq((seq_t)p, 4);
    }
    primes_sq_cnt   = prime_count;
    primes_cube_cnt = prime_count;
    primes_fourth_cnt = prime_count;
}

/* ------------------------------------------------------------------ */

/* shorthand wrappers for the three exponents */
static bool has_square(seq_t num)   { return has_any_prime_power(sq_primes,   primes_sq_cnt,   num, 2);  }
static bool has_cube  (seq_t num)   { return has_any_prime_power(cube_primes, primes_cube_cnt, num, 3);  }
static bool has_fourth(seq_t num)   { return has_any_prime_power(fourth_primes, primes_fourth_cnt, num, 4); }

/* ------------------------------------------------------------------ */
/* Cache of computed sequence values.  The array grows as needed. */
static seq_t *seq_cache   = NULL;
static size_t seq_cache_sz  = 0;
static size_t seq_cache_cap = 0;

/* allocate or grow cache */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= seq_cache_cap) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_cache   = tmp;
    seq_cache_cap = new_cap;
}

/* Next candidate to inspect – starts at 1 and is updated after each term. */
static seq_t next_candidate = 1;

/* ------------------------------------------------------------------ */
/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* for safety – OEIS uses offset 1 */

    init_primes_and_powers();   /* primes and powers ready */

    /* compute terms up to requested index */
    while (seq_cache_sz < (size_t)n)
    {
        seq_t m = next_candidate;
        /* search forward until a valid triple is found */
        for (;; ++m)
        {
            if (!has_square(m))        continue;
            if (!has_cube(m + 1))      continue;
            if (!has_fourth(m + 2))    continue;
            /* found a term */
            ensure_cache_cap(seq_cache_sz + 1);
            seq_cache[seq_cache_sz++] = m;
            next_candidate = m + 1;    /* next search starts after this term */
            break;
        }
    }
    return seq_cache[(size_t)n - 1];
}
