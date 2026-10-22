/*
 * A337395  a(n) is the largest exponent k such that the sums, with
 *          multiplicity, of the i-th powers of the prime factors of
 *          A100118(n) are all prime for i=1 to k.
 * https://oeis.org/A337395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the largest k such that the sums, with multiplicity, of
 *            the i‑th powers of the prime factors of A100118(n) are all prime
 *            for i = 1..k. (Offset 1)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 18, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   factor_u64, is_prime_big, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_pow.h"

/* cache of A100118(n) values */
static uint64_t *b_cache     = NULL;   /* cached values              */
static size_t   b_cap       = 0;        /* allocated capacity         */
static idx_t    b_len       = 0;        /* number of values computed  */
static uint64_t b_next      = 2;        /* next candidate to test     */

/* grow the cache if necessary */
static void ensure_b_capacity(size_t required)
{
    if (required <= b_cap) return;
    size_t new_cap = b_cap ? b_cap * 2 : 16;
    while (new_cap < required) new_cap *= 2;
    b_cache = (uint64_t *)realloc(b_cache, new_cap * sizeof(uint64_t));
    b_cap   = new_cap;
}

/* return the nth element of A100118 (offset 1) */
static uint64_t get_A100118(idx_t n)
{
    while (b_len < n)
    {
        uint64_t primes[32];
        uint64_t exps[32];
        size_t   cnt;

        factor_u64(b_next, primes, exps, &cnt);

        seq_t sum = 0;
        for (size_t i = 0; i < cnt; ++i)
            sum += (seq_t)primes[i] * (seq_t)exps[i];

        if (is_prime_big(sum))
        {
            ensure_b_capacity(b_len + 1);
            b_cache[b_len++] = b_next;
        }
        ++b_next;
    }
    return b_cache[n - 1];
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n=0, return 0 */

    uint64_t val = get_A100118(n);     /* the number from A100118(n) */

    uint64_t primes[32];
    uint64_t exps[32];
    size_t   cnt;

    factor_u64(val, primes, exps, &cnt);

    idx_t k = 0;
    for (idx_t i = 1; ; ++i)
    {
        seq_t sum = 0;
        for (size_t j = 0; j < cnt; ++j)
        {
            seq_t term = seq_pow((seq_t)primes[j], i);
            term *= (seq_t)exps[j];
            sum += term;
        }
        if (!is_prime_big(sum)) break;
        k = i;
    }
    return (seq_t)k;
}
