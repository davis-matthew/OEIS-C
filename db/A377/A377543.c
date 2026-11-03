/*
 * A377543  a(n) = least prime > a(n - 1)*a(n - 3)/a(n - 2), with a(1) = 2,
 *          a(2) = 3, a(3) = 5.
 * https://oeis.org/A377543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=3, a(3)=5; a(n) = least prime >
 *            a(n-1)*a(n-3)/a(n-2) for n≥4
 * Range:     u8 n <= 118, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 989 (2026-11-03)
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

/* Compute ⌊ a * b / c ⌋ using the type seq_t. */
static inline seq_t prod_div(seq_t a, seq_t b, seq_t c) {
    return (seq_t)((a * b) / c);
}

/* Zero‑based cache: cache[0] holds a(1), cache[1] holds a(2), ... */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* Ensure that the cache contains at least n terms (1‑based). */
static void ensure_cache_size(idx_t n) {
    if (cache_size >= (size_t)n)
        return;

    /* Allocate enough space; keep one slot extra to avoid off‑by‑one errors. */
    size_t need = (size_t)n + 1;
    if (cache_cap == 0)
        cache_cap = 8;
    while (cache_cap < need)
        cache_cap <<= 1;

    seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
    if (!tmp) abort();
    cache = tmp;

    /* First allocation: initialise a(1), a(2), a(3). */
    if (cache_size == 0) {
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache_size = 3;
    }

    /* Compute terms until we reach the requested index n. */
    while (cache_size < (size_t)n) {
        seq_t a_n1 = cache[cache_size - 1];   /* a(n-1) */
        seq_t a_n3 = cache[cache_size - 3];   /* a(n-3) */
        seq_t a_n2 = cache[cache_size - 2];   /* a(n-2) */
        seq_t val   = prod_div(a_n1, a_n3, a_n2);  /* floor(a_n1*a_n3/a_n2) */

        /* Least prime greater than val. */
        seq_t cand = val + (seq_t)1;
        if (cand <= (seq_t)2) {
            cand = (seq_t)2;
        } else {
            if ((cand & (seq_t)1) == (seq_t)0)
                cand = cand + (seq_t)1;           /* make cand odd */
            while (!is_prime_big(cand))
                cand = cand + (seq_t)2;           /* skip even numbers */
        }

        cache[cache_size] = cand;
        ++cache_size;
    }
}

/* Public function: return a(n).  n is 1‑based. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0 */
    ensure_cache_size(n);
    return cache[n - 1];
}
