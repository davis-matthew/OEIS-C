/*
 * A335904  Fully additive with a(2) = 0, and a(p) = 1+a(p-1)+a(p+1), for odd
 *          primes p.
 * https://oeis.org/A335904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9286, u16 n <= 9286, u32 n <= 9286, u64 n <= 9286,
 *            u128 n <= 9286, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * a(1) = 0 , a(2) = 0.
 * For every odd prime p:          a(p) = 1 + a(p-1) + a(p+1).
 * a is totally additive:          a(m*n) = a(m) + a(n)  for coprime m,n.
 *
 * The implementation memoises all previously computed values
 * and uses trial factorisation for composite arguments.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

static seq_t *a_cache      = NULL;   /* memoised values */
static uint8_t *a_done     = NULL;   /* 0 = unset, 1 = set */
static size_t a_cache_size = 0;      /* current allocated size */

/* ensure that indices up to n are available in the cache */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > a_cache_size) {
        size_t new_size = needed * 2 + 10;
        a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        a_done  = (uint8_t *)realloc(a_done,  new_size * sizeof(uint8_t));
        for (size_t i = a_cache_size; i < new_size; ++i) a_done[i] = 0;
        a_cache_size = new_size;
    }
}

/* main sequence function */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    if (a_done[n]) return a_cache[n];

    seq_t val = 0;

    if (n == 1) {
        val = 0;
    } else if (is_prime(n)) {
        if (n == 2) {
            val = 0;
        } else {
            idx_t left  = (n - 1) / 2;   /* (p-1)/2 for odd prime p */
            idx_t right = (n + 1) / 2;   /* (p+1)/2 for odd prime p */
            val = a(left) + a(right) + 1;
        }
    } else {
        /* composite: a(n) = sum e_i * a(p_i) over prime power factors */
        uint64_t primes[64];
        uint64_t expsU[64];
        size_t cnt = 0;
        factor_u64((uint64_t)n, primes, expsU, &cnt);
        for (size_t i = 0; i < cnt; ++i) {
            seq_t p_val = a((idx_t)primes[i]);              /* a(p_i)   */
            seq_t mult  = p_val * (seq_t)expsU[i];          /* e_i * a(p_i) */
            val += mult;
        }
    }

    a_cache[n] = val;
    a_done[n]  = 1;
    return val;
}
