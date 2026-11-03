/*
 * A252005  a(1)=2; for n>1, a(n) is the least prime > a(n-1) such that sum of
 *          digits of a(n-1) and a(n) is prime.
 * https://oeis.org/A252005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 883 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; for n>1, a(n) is the least prime > a(n-1) such that
   digit_sum(a(n-1))+digit_sum(a(n)) is prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
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

/* Return the smallest prime strictly greater than start. */
static seq_t next_prime_after(seq_t start)
{
    seq_t cand = start + (seq_t)1;
    while (!is_prime_big(cand))
        ++cand;
    return cand;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined below offset; return 0 for safety */
        return 0;

    static seq_t *cache = NULL;   /* 0‑indexed array of computed terms */
    static idx_t cache_len = 0;   /* number of terms stored in cache */

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Expand cache to hold term n */
    size_t new_len = (size_t)n;
    seq_t *new_cache = mem_resize(seq_t, cache, new_len);
    if (!new_cache) abort();        /* allocation failure */
    cache = new_cache;

    /* Compute missing terms sequentially */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)2;
            continue;
        }

        seq_t prev = cache[i - 2];   /* previous term (0‑based) */

        /* Find the least prime > prev that satisfies the digit‑sum condition */
        seq_t cand = next_prime_after(prev);
        while (true) {
            seq_t sum = digit_sum(prev) + digit_sum(cand);
            if (is_prime_big(sum))
                break;
            cand = next_prime_after(cand);
        }

        cache[i - 1] = cand;
    }

    cache_len = n;
    return cache[n - 1];
}
