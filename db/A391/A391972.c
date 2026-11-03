/*
 * A391972  a(n) = 1 if n=1 or a(n-1) is prime; a(n) = a(n-1) + i otherwise,
 *          where i = n - r and r is the number of occurrences of 1 among
 *          terms a(1) to a(n-1).
 * https://oeis.org/A391972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 1 or a(n-1) is prime; a(n) = a(n-1) + i
 *            otherwise, where i = n - r and r is the number of occurrences of
 *            1 among terms a(1) to a(n-1)
 * Range:     u8 n <= 41, u16 n <= 872, u32 n <= 10015, u64 n <= 10015,
 *            u128 n <= 10015, big any n
 * Validated: standard, pass, 86 terms through n = 9398 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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
    if (n == 0) return (seq_t)0;           /* out of range (offset = 1) */

    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* number of terms cached */
    static size_t ones  = 0;   /* number of 1's among cached terms */

    /* Guard: if needed, extend cache */
    if (n > cap) {
        /* allocate or reallocate */
        if (cap == 0) {
            cache = mem_new(seq_t, 1);
            if (!cache) abort();          /* allocation failed */
            cache[0] = (seq_t)1;          /* a(1) = 1 */
            cap      = 1;
            ones     = 1;
        } else {
            seq_t *tmp = mem_resize(seq_t, cache, (size_t)n);
            if (!tmp) abort();
            cache = tmp;
        }

        /* compute missing terms */
        for (idx_t i = (idx_t)cap + 1; i <= n; ++i) {
            seq_t prev = cache[(size_t)i - 2];  /* a(i-1) */
            seq_t cur;

            if (is_prime_big(prev))          /* reset to 1 on prime */
                cur = (seq_t)1;
            else {
                /* step = i - r (r = ones count so far) */
                seq_t step = (seq_t)((idx_t)i - ones);
                cur = prev + step;
            }

            cache[(size_t)i - 1] = cur;
            if (cur == (seq_t)1) ++ones;
        }
        cap = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
