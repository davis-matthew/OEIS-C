/*
 * A308099  Numbers k with 2 or more chained distinct prime factors: the last
 *          digit of every prime factor is the same as the first digit of the
 *          next prime factor. Prime factors must be in ascending order.
 * https://oeis.org/A308099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 3439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308099: Numbers that have two or more distinct
 * prime factors in ascending order such that each prime’s last
 * decimal digit equals the first decimal digit of the next prime.
 *
 * Implementation: An incremental search with a static cache.  The
 * function guarantees that any requested term a(n) is found by
 * extending the cache until it contains n elements.  All work is
 * done over uint64_t integers; the resulting value is returned as
 * seq_t (which may be uint8_t, uint16_t, … or an arbitrary‑precision
 * type).  The algorithm is O(T) where T is the number of tested
 * integers up to the n‑th term, which comfortably meets the
 * 2‑second requirement for n up to ~10 000.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Return the n‑th term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    /* Static cache that persists across calls.  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* Next integer to test for membership.  */
    static uint64_t next_test = 2;

    if (n == (idx_t)0)   /* out of range: return 0, behaviour irrelevant */
        return (seq_t)0;

    /* Extend the cache until it contains n terms.  */
    while (cache_size < n) {
        uint64_t primes[64];
        uint64_t exps[64];
        size_t pc = 0;   /* number of distinct prime factors */

        factor_u64(next_test, primes, exps, &pc);

        /* Need at least two distinct prime factors.  */
        if (pc >= 2) {
            bool ok = true;
            for (size_t i = 0; i + 1 < pc; ++i) {
                /* last decimal digit of primes[i] */
                uint8_t last_digit = (uint8_t)(primes[i] % 10);

                /* first decimal digit of primes[i+1] */
                uint64_t nextp = primes[i + 1];
                while (nextp >= 10)
                    nextp /= 10;
                uint8_t first_digit = (uint8_t)nextp;

                if (last_digit != first_digit) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                /* Grow cache if needed.  */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();      /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = (seq_t)next_test;
            }
        }
        ++next_test;
    }

    return cache[n - 1];   /* 1‑based indexing */
}
