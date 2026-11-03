/*
 * A289839  Primes of the form 8*n^2+8*n+31.
 * https://oeis.org/A289839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A289839: primes of the form 8*n^2+8*n+31 (n≥0).
   a(1) corresponds to n = 0. */
#include <stddef.h>
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
    /* static memoisation state */
    static seq_t *cache = NULL;          /* cached prime values          */
    static size_t cache_len = 0;         /* number of cached values      */
    static size_t cache_cap = 0;         /* allocated capacity           */
    static idx_t next_k = 0;             /* next n to test (0‑based)     */

    /* a(1) is the first term; 0 is undefined – return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Generate terms until we have at least n cached. */
    while (cache_len < (size_t)n) {
        idx_t k = next_k;
        /* compute 8*k^2 + 8*k + 31 using a wide type to avoid overflow */
        unsigned __int128 vbig = (unsigned __int128)8 * k * k
                               + (unsigned __int128)8 * k
                               + (unsigned __int128)31;
        seq_t val = (seq_t)vbig;         /* cast into seq_t (may wrap if too large) */

        if (is_prime_big(val)) {
            /* enlarge the cache if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_k;
    }

    /* return the requested term (1‑based index) */
    return cache[(size_t)(n - 1)];
}
