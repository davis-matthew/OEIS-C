/*
 * A389458  Numbers whose sum of non-unitary divisors (A048146) is congruent
 *          to 2 modulo 4.
 * https://oeis.org/A389458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers whose sum of non-unitary divisors is ≡ 2 (mod 4)
 * Range:     u8 n <= 15, u16 n <= 2405, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7365, big any n
 * Helpers:   factor_u64, gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t, uint64_t *, uint64_t *, size_t *) */
#include "../utils/gcd_uint64.h"

/* Compute the sum of non‑unitary divisors of n (n >= 2) */
static uint64_t sum_nonunitary_divisors(uint64_t n)
{
    /* Factor n */
    uint64_t primes[64];          /* enough for 64‑bit n */
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    uint64_t sum = 0;

    /* Iterative traversal of all divisors */
    struct Work { size_t idx; uint64_t d; };
    struct Work stack[128];
    size_t sp = 0;
    stack[sp++] = (struct Work){0, 1};

    while (sp) {
        struct Work w = stack[--sp];
        if (w.idx == cnt) {
            if (w.d > 1 && w.d < n) {
                uint64_t e = n / w.d;
                if (gcd_uint64(w.d, e) > 1) sum += w.d;
            }
            continue;
        }

        uint64_t p = primes[w.idx];
        uint64_t d = w.d;
        for (uint64_t e = 0; e <= exps[w.idx]; ++e) {
            stack[sp++] = (struct Work){w.idx + 1, d};
            if (e == exps[w.idx]) break;
            d *= p;
        }
    }

    return sum;
}

/* Test whether n satisfies the condition */
static bool satisfies(uint64_t n)
{
    uint64_t s = sum_nonunitary_divisors(n);
    return ((s % 4ULL) == 2ULL);
}

seq_t a(idx_t n)
{
    /* Cache of found terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    /* Next integer to test */
    static uint64_t candidate = 2;

    /* Expand cache if needed */
    if (len == cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Find terms until we have the n‑th one */
    while (len < (size_t)n) {
        if (satisfies(candidate)) {
            cache[len++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return cache[(size_t)n - 1];
}
