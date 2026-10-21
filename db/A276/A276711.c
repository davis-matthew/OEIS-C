/*
 * A276711  Number of primes p <= n such that n - p is a perfect power
 *          (including 0 and 1).
 * https://oeis.org/A276711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A276711
   a(n) = number of primes p ≤ n such that n – p is a perfect power
   (including 0 and 1).  The function builds a prime sieve and a
   perfect‑power table up to the requested n; both tables are rebuilt
   only when a larger n is requested.  The routine runs in O(n)
   time for n up to (and comfortably beyond) 10 000.  It works for
   arbitrary‑precision seq_t, for any integer size of idx_t (uint64_t),
   and for all the helper functions defined in the project.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ----------------------------------------------------------------------
 * Prime sieve – rebuilt only when a larger limit is required.
 * ---------------------------------------------------------------------- */
static uint64_t *prime_list   = NULL;   /* prime numbers, ascending order */
static size_t   prime_cnt     = 0;      /* number of primes in the list      */
static idx_t    sieve_limit   = 0;      /* current maximum index sieved      */

/* rebuild_sieve up to new_limit (inclusive).  All arrays are allocated
   freshly; previous memory is freed.  The function is O(new_limit log log new_limit). */
static void rebuild_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit) return;          /* nothing to do */

    /* allocate boolean array for composite markers: 0 = prime, 1 = composite */
    uint8_t *is_comp = (uint8_t *)calloc((size_t)(new_limit + 1), 1);
    if (!is_comp) exit(EXIT_FAILURE);

    /* sieve of Eratosthenes */
    idx_t sq = (idx_t) ((double)new_limit * 0.5);   /* over‑estimate for safety */
    if (sq > new_limit) sq = new_limit;
    for (idx_t i = 2; i <= sq; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (!is_comp[i]) ++cnt;

    /* build prime list */
    uint64_t *new_primes = (uint64_t *)realloc(prime_list, cnt * sizeof(uint64_t));
    if (!new_primes) exit(EXIT_FAILURE);

    size_t pos = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (!is_comp[i]) new_primes[pos++] = (uint64_t)i;

    /* free old data and replace */
    free(is_comp);
    prime_list   = new_primes;
    prime_cnt    = cnt;
    sieve_limit  = new_limit;
}

/* ----------------------------------------------------------------------
 * Perfect‑power table – keeps a bit array indicating which numbers
 * up to a limit are perfect powers.  The table is rebuilt only when
 * a larger limit is requested.  It includes 0 and 1 (exponents ≥ 2).
 * ---------------------------------------------------------------------- */
static uint8_t *power_flag  = NULL;   /* 0/1 flag for each integer up to limit */
static idx_t    pp_limit    = 0;      /* current maximum index of power_flag */

/* Ensure power_flag contains flags for all integers ≤ new_limit. */
static void ensure_power_flag(idx_t new_limit)
{
    if (new_limit <= pp_limit) return;          /* nothing to do */

    /* Allocate new flag array and zero it */
    uint8_t *new_flag = (uint8_t *)calloc((size_t)(new_limit + 1), 1);
    if (!new_flag) exit(EXIT_FAILURE);

    /* Generate perfect powers up to new_limit and set flags */
    for (idx_t base = 0; base <= new_limit; ++base) {
        if (base == 0 || base == 1) {
            new_flag[base] = 1;
            continue;
        }
        uint64_t val = (uint64_t)base * (uint64_t)base;       /* base² */
        while (val <= (uint64_t)new_limit) {
            new_flag[(size_t)val] = 1;
            if (val > (uint64_t)new_limit / (uint64_t)base)   /* would overflow */
                break;
            val *= (uint64_t)base;
        }
    }

    /* Free old flag array */
    free(power_flag);
    power_flag = new_flag;
    pp_limit   = new_limit;
}

/* ----------------------------------------------------------------------
 * Main sequence function.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset starts at 1 */

    /* Build tables up to n if necessary */
    rebuild_sieve(n);
    ensure_power_flag(n);

    /* Count primes p ≤ n with n – p a perfect power */
    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > (uint64_t)n) break;
        idx_t diff = (idx_t)(n - (idx_t)p);
        if (power_flag[diff])
            ++count;
    }
    return count;
}
