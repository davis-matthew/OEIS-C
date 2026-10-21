/*
 * A258436  Primes p of form x^2 - phi(x) such that (p-1)/tau(p-1) is also
 *          prime.
 * https://oeis.org/A258436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime p of the form p = x^2 − φ(x) * such that
 *            (p − 1)/τ(p − 1) is also prime, where τ is the number * of
 *            divisors. (OEIS A258436)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   calc_phi, divisor_count_u64, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t) */
#include "../utils/calc_phi.h"         /* seq_t calc_phi(seq_t) */
#include "../utils/divisor_count_u64.h"/* uint64_t divisor_count_u64(uint64_t) */
#include "../utils/grow_seq.h"         /* void grow_seq(seq_t **, size_t *, size_t) */

static seq_t *cache   = NULL;   /* dynamic array of found primes */
static size_t cache_cap = 0;    /* current capacity of the array */
static idx_t   cache_size = 0;  /* number of primes stored */
static idx_t   next_x = 2;      /* next x to test */

/* Append a prime to the cache, growing the array if needed. */
static void append_prime(seq_t p)
{
    /* Ensure there is space for one more element. */
    grow_seq(&cache, &cache_cap, 1);
    cache[cache_size++] = p;
}

/* Generate primes until at least n terms are stored. */
static void generate_until(idx_t n)
{
    while (cache_size < n)
    {
        seq_t x        = (seq_t)next_x;
        seq_t phi_x    = calc_phi(x);
        seq_t p        = x * x - phi_x;            /* candidate prime */

        /* p must be > 0 and fit into 64‑bit for the divisor test */
        if (p > (seq_t)0 && p <= (seq_t)UINT64_MAX)
        {
            if (is_prime_big(p))
            {
                uint64_t p_minus1 = (uint64_t)(p - 1);
                uint64_t tau     = divisor_count_u64(p_minus1);

                /* tau must divide (p-1) exactly */
                if (tau != 0 && p_minus1 % tau == 0)
                {
                    uint64_t ratio = p_minus1 / tau;
                    if (is_prime_big((seq_t)ratio))
                    {
                        append_prime(p);
                    }
                }
            }
        }

        ++next_x;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; undefined for 0 */
    generate_until(n);
    return cache[n - 1];
}
