/*
 * A347644  Number of partitions of n into at most 3 prime powers (including
 *          1).
 * https://oeis.org/A347644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most 3 prime powers
 *            (including 1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function enumerates unordered triples of prime powers summing to n. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache of prime powers up to the greatest n requested. */
static idx_t _max_pp = 0;          /* maximum n for which we have prime‑powers data */
static size_t _pp_len = 0;         /* number of distinct prime powers (incl. 1) */
static uint64_t *_pp_vals = NULL;  /* sorted list of prime powers */
static uint8_t  *_pp_mem  = NULL;  /* bitmap: 1 if number is a prime power */

/* Build the list and bitmap of prime powers up to 'limit'.  */
static void _rebuild_prime_powers(idx_t limit)
{
    /* free old data */
    free(_pp_vals);
    free(_pp_mem);

    /* allocate temporary arrays */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    uint8_t *is_pp    = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!is_prime || !is_pp) exit(1);  /* memory error */

    /* simple sieve of Eratosthenes (mark composites) */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 1;

    /* mark prime powers */
    for (idx_t p = 2; p <= limit; ++p)
        if (!is_prime[p]) {          /* p is prime */
            uint64_t t = p;
            while (t <= limit) {
                is_pp[t] = 1;
                if (t > ((uint64_t)limit) / p) break;  /* avoid overflow */
                t *= p;
            }
        }
    /* include 1 explicitly */
    is_pp[1] = 1;

    /* collect distinct values into array */
    size_t c = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (is_pp[i]) ++c;

    _pp_vals = (uint64_t *)malloc(c * sizeof(uint64_t));
    if (!_pp_vals) exit(1);
    size_t pos = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (is_pp[i]) _pp_vals[pos++] = (uint64_t)i;

    _pp_mem   = is_pp;          /* keep bitmap (will be freed next rebuild) */
    _pp_len   = c;
    _max_pp   = limit;

    free(is_prime);
    /* is_pp is now owned by _pp_mem and will be freed next time */
}

/* Ensure we have prime powers up to at least n. */
static void _ensure_pp(idx_t n)
{
    if (n > _max_pp) _rebuild_prime_powers(n);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;            /* a(0) = 1 by definition */

    _ensure_pp(n);

    seq_t count = 0;

    /* 1‑part partitions: only if n itself is a prime power */
    if (n <= _max_pp && _pp_mem[n]) count += (seq_t)1;

    /* 2‑part partitions */
    for (size_t i = 0; i < _pp_len; ++i) {
        idx_t a_val = (idx_t)_pp_vals[i];
        if (a_val > n / 2) break;
        idx_t b_val = n - a_val;
        if (b_val < a_val) continue;
        if (b_val <= _max_pp && _pp_mem[b_val]) count += (seq_t)1;
    }

    /* 3‑part partitions */
    for (size_t i = 0; i < _pp_len; ++i) {
        idx_t a_val = (idx_t)_pp_vals[i];
        if (a_val > n) break;
        for (size_t j = i; j < _pp_len; ++j) {
            idx_t b_val = (idx_t)_pp_vals[j];
            if (a_val + b_val > n) break;
            idx_t c_val = n - a_val - b_val;
            if (c_val < b_val) continue;         /* enforce a <= b <= c */
            if (c_val <= _max_pp && _pp_mem[c_val]) count += (seq_t)1;
        }
    }

    return count;
}
