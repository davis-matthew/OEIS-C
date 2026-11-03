/*
 * A258663  Numbers n such that 9n-1 is prime.
 * https://oeis.org/A258663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer m such that 9m - 1 is prime. *
 *            Computation: trial‑division primality testing of the value 9m-1.
 *            * Results are cached; any ordering of calls is supported
 * Range:     u8 n <= 10, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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

static seq_t *cache = NULL;      /* cached a(k) values, 1‑based indexing */
static size_t cache_len = 0;     /* number of cached values (cache_len >= a(n) for computed n) */
static size_t cache_cap = 0;     /* capacity of the cache array */
static seq_t next_candidate = 2; /* next m to test, starts at 2 (first term) */

/* Expand cache capacity to at least new_cap */
static void ensure_cache_cap(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_cap_extended = new_cap * 2 + 10;
    seq_t *new_ptr = mem_resize(seq_t, cache, new_cap_extended);
    if (!new_ptr) {
        /* allocation failure: terminate */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap_extended;
}

/* Generate terms until cache_len >= n */
static void generate_terms(idx_t n)
{
    while (cache_len < (size_t)n) {
        seq_t m = next_candidate++;
        seq_t candidate = (seq_t)9 * m - (seq_t)1;
        if (is_prime_big(candidate)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = m;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined, return 0 */
    if (cache == NULL) {
        /* lazy initialization */
        cache_cap = 20;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
    }
    generate_terms(n);
    return cache[n - 1];
}
