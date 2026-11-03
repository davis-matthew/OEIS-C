/*
 * A284383  a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 2; a(n) is the largest
 *          composite number <= (a(n-a(n-1)) + a(n-a(n-2))) for n > 4.
 * https://oeis.org/A284383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 2; for n > 4: a(n) =
 *            largest composite ≤ (a(n-a(n-1)) + a(n-a(n-2)))
 * Range:     u8 n <= 380, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9486 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

/* Memoisation table: index 1..N stored in cache[1..N] */
static seq_t *cache       = NULL;
static idx_t  cache_cap   = 0;   /* allocated size (≥ cache_computed+1) */
static idx_t  cache_computed = 0;/* highest n already computed */

seq_t a(idx_t n)
{
    /* n is always ≥ 1; guard for 0 just in case */
    if (n == 0) return 0;

    /* grow the cache if necessary (C++ requires explicit cast) */
    if (cache_cap < n + 1) {
        seq_t *tmp = mem_resize(seq_t, cache, (n + 1));
        if (!tmp) return 0;        /* allocation failed – return 0 */
        cache      = tmp;
        cache_cap  = n + 1;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = 1;
        } else if (i == 2) {
            cache[i] = 2;
        } else if (i == 3) {
            cache[i] = 3;
        } else if (i == 4) {
            cache[i] = 2;
        } else {
            /* recurrence indices (indices are 1‑based) */
            idx_t idx1 = i - (idx_t)cache[i - 1];
            idx_t idx2 = i - (idx_t)cache[i - 2];

            seq_t sum = cache[idx1] + cache[idx2];
            seq_t val = sum;

            /* find the largest composite ≤ sum */
            while (val > 1 && is_prime_big(val)) {
                val = val - (seq_t)1;
            }
            cache[i] = val;   /* will be composite or 0 if sum < 2 */
        }
    }
    cache_computed = n;
    return cache[n];
}
