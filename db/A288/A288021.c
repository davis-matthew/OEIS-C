/*
 * A288021  Prime p1 of consecutive primes p1, p2, where p2 - p1 = 4, and p1,
 *          p2 are in different decades.
 * https://oeis.org/A288021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that p+4 is prime, p+2 composite,
 *            and (p/10)!=(p+4)/10, i.e. consecutive primes with difference 4
 *            spanning a decade
 * Range:     u8 n <= 9, u16 n <= 563, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 996 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 7;

static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
    if (!new_ptr) abort();
    cache = new_ptr;
    cache_cap = new_cap;
}

static void extend_cache_to(idx_t n)
{
    while (cache_len < n)
    {
        seq_t candidate = next_candidate;
        if (is_prime_big(candidate))
        {
            seq_t next = candidate + 4;
            if (is_prime_big(next) &&
                !is_prime_big(candidate + 2) &&
                (candidate / 10) != (next / 10))
            {
                grow_cache(cache_len + 1);
                cache[cache_len++] = candidate;
            }
        }
        next_candidate = candidate + 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, undefined for n<1 */
    extend_cache_to(n);
    return cache[n - 1];
}
