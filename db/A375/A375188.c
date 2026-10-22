/*
 * A375188  Number of non-unitary square divisors of n!.
 * https://oeis.org/A375188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of non‑unitary square divisors of n!
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 97, u64 n <= 235,
 *            u128 n <= 565, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = ∏p (⌊e_p/2⌋+1) – ∏p|e_p even 2,   where e_p = exponent of p in n! */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t  */
#include <stdlib.h>   /* malloc, free, size_t */

/* Global static data for prime caching */
static idx_t  prime_limit = 0;          /* largest n for which primes are pre‑computed */
static uint64_t *prime_tbl = NULL;      /* prime list (up to prime_limit) */
static size_t  prime_cnt = 0;           /* number of primes in prime_tbl */

/* Helper: recompute the list of primes up to n (inclusive) */
static void compute_primes(idx_t n) {
    if (n <= prime_limit) return;           /* already have enough primes */

    /* allocate sieve array */
    size_t sz = (size_t)(n + 1);
    unsigned char *is_composite = (unsigned char *)malloc(sz);
    if (!is_composite) return;              /* allocation failure: give up */

    for (size_t i = 0; i < sz; ++i) is_composite[i] = 0;
    is_composite[0] = is_composite[1] = 1;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (!is_composite[(size_t)p]) {
            for (idx_t q = p * p; q <= n; q += p) {
                is_composite[(size_t)q] = 1;
            }
        }
    }

    /* count primes to allocate prime_tbl */
    size_t new_cnt = 0;
    for (idx_t i = 2; i <= n; ++i) if (!is_composite[(size_t)i]) ++new_cnt;

    uint64_t *new_tbl = (uint64_t *)realloc(prime_tbl, new_cnt * sizeof(uint64_t));
    if (!new_tbl) { free(is_composite); return; } /* allocation failure */
    prime_tbl = new_tbl;
    prime_cnt = new_cnt;

    size_t idx = 0;
    for (idx_t i = 2; i <= n; ++i) if (!is_composite[(size_t)i]) {
        prime_tbl[idx++] = (uint64_t)i;
    }

    prime_limit = n;
    free(is_composite);
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Special case: n = 0 or 1 gives 0 */
    if (n < 2) return (seq_t)0;

    /* Ensure we have primes up to n */
    compute_primes(n);

    seq_t total = (seq_t)1;      /* product of (floor(e/2)+1) */
    seq_t unitary = (seq_t)1;    /* product over primes with even exponent of 2 */

    /* For each prime p <= n */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        if (p > n) break;

        /* Legendre's formula for exponent of p in n! */
        idx_t e = 0;
        idx_t power = p;
        while (power <= n) {
            e += n / power;
            if (power > n / p) break;   /* avoid overflow of power*p */
            power *= p;
        }

        /* Multiply factor (floor(e/2)+1) into total */
        seq_t factor = (seq_t)(e / 2) + (seq_t)1;
        total *= factor;

        /* If exponent is even, multiply unitary by 2 */
        if (e % 2 == 0) unitary *= (seq_t)2;
    }

    /* Subtract unitary square divisors (including 1) */
    return total - unitary;
}
