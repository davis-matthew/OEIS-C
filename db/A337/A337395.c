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
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 88 (2026-11-03)
 * Helpers:   factor_u64, ipow_idx, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
            seq_t term = ipow_idx((seq_t)primes[j], i);
            term *= (seq_t)exps[j];
            sum += term;
        }
        if (!is_prime_big(sum)) break;
        k = i;
    }
    return (seq_t)k;
}
