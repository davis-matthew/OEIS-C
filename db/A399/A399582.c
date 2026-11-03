/*
 * A399582  Numbers k not divisible by 3 such that at least one of 3+2k and
 *          3+4k is composite.
 * https://oeis.org/A399582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k>0 with k%3!=0 and (3+2k or 3+4k is
 *            composite)
 * Range:     u8 n <= 23, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8796 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterative search with memoization; uses is_prime_big for primality checks. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"
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

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored in cache */
    static idx_t next_k = 1;        /* next k to test for inclusion */

    if (n == 0)
        return 0;   /* undefined, but safe to return zero */

    /* Grow cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        /* search for next qualifying k */
        while (true) {
            idx_t k = next_k++;
            if (k % 3 == 0)
                continue;    /* skip multiples of 3 */

            seq_t val1 = 3 + (seq_t)2 * k;
            seq_t val2 = 3 + (seq_t)4 * k;

            if (!is_prime_big(val1) || !is_prime_big(val2)) {
                /* Store the qualifying k */
                if (cache_size == 0) {
                    cache = mem_new(seq_t, 1);
                } else {
                    cache = mem_resize(seq_t, cache, (cache_size + 1));
                }
                cache[cache_size++] = (seq_t)k;
                break;   /* move to next term needed */
            }
        }
    }

    return cache[n - 1];
}
