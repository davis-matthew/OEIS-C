/*
 * A326708  Non-Brazilian squares of primes.
 * https://oeis.org/A326708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Non-Brazilian squares of primes.
   a(n) = (prime)^2 where the prime is the n-th prime
   excluding 11 (the only Brazilian prime square). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Static storage for prime sieve */
static uint64_t *primes = NULL;
static size_t prime_count = 0;
static size_t curr_limit   = 0;
static bool   initialized  = false;

/* Build all primes up to \a limit using a simple sieve of Eratosthenes.
   The primes are stored in the static array \a primes. */
static void generate_primes(size_t limit)
{
    /* Allocate composite flag array */
    bool *is_comp = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_comp) return;                 /* allocation failure – give up */

    /* Mark composites */
    for (size_t p = 2; (size_t)p * p <= limit; ++p) {
        if (!is_comp[p]) {
            for (size_t m = p * p; m <= limit; m += p)
                is_comp[m] = true;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;

    /* Allocate array of primes */
    uint64_t *arr = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!arr) { free(is_comp); return; }

    /* Store primes */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) arr[idx++] = i;
    free(is_comp);

    /* Replace previous prime list */
    if (primes) free(primes);
    primes     = arr;
    prime_count = cnt;
    curr_limit  = limit;
}

/* Make sure the prime list contains at least \a needed primes
   when 11 is excluded.  The list is grown by doubling its
   upper bound until sufficient. */
static void ensure_primes(idx_t needed)
{
    if (!initialized) {
        generate_primes(200000);   /* initial size: 17984 primes */
        initialized = true;
    }

    /* Number of usable primes (exclude 11 if present in range) */
    size_t effective = prime_count - ((curr_limit >= 11) ? 1 : 0);

    while (effective < (size_t)needed) {
        generate_primes(curr_limit * 2);
        effective = prime_count - ((curr_limit >= 11) ? 1 : 0);
    }
}

/* OEIS a sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, caller guarantees n>=1 */

    ensure_primes(n);

    size_t found = 0;
    uint64_t p = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        if (primes[i] == 11) continue;   /* skip the Brazilian prime */
        ++found;
        if (found == n) {
            p = primes[i];
            break;
        }
    }

    return (seq_t)p * (seq_t)p;
}
