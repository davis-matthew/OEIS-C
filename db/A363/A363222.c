/*
 * A363222  Numbers whose multiset of prime indices satisfies (maximum) -
 *          (minimum) = (length).
 * https://oeis.org/A363222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer whose multiset of prime indices
 *            satisfies (max index) - (min index) = (length of the multiset)
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function scans integers, factorises them using a smallest‑prime
   factor sieve up to the current upper bound, and keeps a dynamic
   upper bound that grows as needed. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* s.f.p. table and prime‑index table (1‑based).  They are rebuilt
   when the required upper bound is exceeded. */
static uint32_t *spf_table = NULL;      /* smallest prime factor of i */
static uint32_t *prime_index = NULL;    /* index of prime i (0 if composite) */
static size_t   limit = 0;              /* current upper bound of the tables */
static bool     tables_built = false;   /* have the tables been initialised? */

/* ------------------------------------------------------------------ */
/* Build the tables up to @max_limit (inclusive). */
static void rebuild_tables(size_t max_limit)
{
    /* free old tables, if any */
    if (spf_table) free(spf_table);
    if (prime_index) free(prime_index);

    /* allocate new tables */
    spf_table   = (uint32_t*)calloc(max_limit + 1, sizeof(uint32_t));
    prime_index = (uint32_t*)calloc(max_limit + 1, sizeof(uint32_t));

    /* linear sieve – record primes and smallest prime factor */
    size_t  primes_cap   = (max_limit / 10) + 32;          /* generous */
    uint32_t *primes     = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    size_t   primes_cnt  = 0;

    for (size_t i = 2; i <= max_limit; ++i) {
        if (!spf_table[i]) {
            spf_table[i] = (uint32_t)i;
            prime_index[i] = (uint32_t)(primes_cnt + 1);   /* 1‑based index */
            if (primes_cnt == primes_cap) {               /* grow the array */
                primes_cap *= 2;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t pj = 0; pj < primes_cnt; ++pj) {
            uint32_t p = primes[pj];
            if (p > spf_table[i]) break;
            size_t ip = i * (size_t)p;
            if (ip > max_limit) break;
            spf_table[ip] = p;
        }
    }
    free(primes);
    tables_built = true;
}

/* ------------------------------------------------------------------ */
/* Test whether the integer x satisfies the required condition. */
static inline bool satisfies(uint64_t x)
{
    uint64_t temp = x;
    uint32_t k = 0;                      /* length of the multiset */
    uint32_t minIdx = UINT32_MAX;
    uint32_t maxIdx = 0;

    while (temp > 1) {
        uint32_t p   = spf_table[(size_t)temp];
        uint32_t idx = prime_index[p];
        /* Count multiplicity of p */
        while (temp % p == 0) {
            ++k;
            if (idx < minIdx) minIdx = idx;
            if (idx > maxIdx) maxIdx = idx;
            temp /= p;
        }
    }
    return (maxIdx - minIdx == k);
}

/* ------------------------------------------------------------------ */
/* Public sequence function.  The first term (n==1) is 10. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset is 1 – should never be called with 0 */

    /* initial build – use a modest bound, grow later if necessary */
    if (!tables_built) {
        limit = 1024;              /* start small to keep memory usage low */
        rebuild_tables(limit);
    }

    seq_t count = 0;
    uint64_t number = 2;

    for (;;) {
        /* If the number exceeds the current upper bound, extend the tables. */
        if (number > limit) {
            size_t new_limit = limit * 2;
            if (new_limit < (size_t)number) new_limit = (size_t)number;
            rebuild_tables(new_limit);
            limit = new_limit;
        }
        if (satisfies(number)) {
            ++count;
            if (count == n) return (seq_t)number;
        }
        ++number;
    }
}
