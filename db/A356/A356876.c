/*
 * A356876  Binary weight of the composite numbers (A002808).
 * https://oeis.org/A356876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = A000120(A002808(n)) (binary weight of the nth composite)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached data */
static seq_t *composites = NULL;          /* popcount of composites */
static size_t comps_len   = 0;            /* number stored */
static size_t comps_cap   = 0;            /* allocated capacity */
static uint64_t current_scan = 2;         /* next integer to test for composite */

static bool *prime_sieve = NULL;          /* prime sieve */
static uint64_t sieve_limit = 0;          /* largest number sieved */

/* popcount of a 64‑bit value, cast to seq_t */
static seq_t popcount_uint64(uint64_t v)
{
    uint64_t c = 0;
    while (v) { v &= v - 1; ++c; }
    return (seq_t)c;
}

/* Expand the prime sieve to cover all numbers up to 'limit' */
static void ensure_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;

    size_t new_cap = (size_t)limit + 1;
    bool *new_sieve = (bool *)calloc(new_cap, sizeof(bool));
    if (!new_sieve) exit(1);

    for (size_t i = 2; i < new_cap; ++i) new_sieve[i] = true;

    for (size_t i = 2; (uint64_t)i * (uint64_t)i <= limit; ++i) {
        if (new_sieve[i]) {
            for (uint64_t j = (uint64_t)i * (uint64_t)i; j <= limit; j += i) {
                new_sieve[(size_t)j] = false;
            }
        }
    }

    free(prime_sieve);
    prime_sieve = new_sieve;
    sieve_limit = limit;
}

/* Grow the composites array until it contains at least 'need' entries */
static void ensure_composites(size_t need)
{
    while (comps_len < need) {
        if (current_scan > sieve_limit) {
            uint64_t new_limit = sieve_limit ? sieve_limit * 2 : 64;
            ensure_sieve(new_limit);
        }
        if (current_scan >= 4 && !prime_sieve[(size_t)current_scan]) {
            if (comps_len == comps_cap) {
                size_t new_cap = comps_cap ? comps_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                composites = tmp;
                comps_cap = new_cap;
            }
            composites[comps_len++] = popcount_uint64(current_scan);
        }
        ++current_scan;
    }
}

/* OEIS function definition */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* out of bounds for offset */

    size_t idx = (size_t)n;
    ensure_composites(idx);
    return composites[idx - 1];
}
