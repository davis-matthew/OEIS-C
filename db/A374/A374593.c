/*
 * A374593  Numbers k such that k - rad(k) + 1 is prime, where rad(k) is the
 *          radical A007947(k).
 * https://oeis.org/A374593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that k - rad(k) + 1 is prime
 * Range:     u8 n <= 59, u16 n <= 7128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 10000 (2026-11-03)
 * Helpers:   is_prime_u64, mem, radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 4, a(2) = 8, a(3) = 9, a(4) = 12, ... */
/* The function caches the computed terms so that each value is only
   evaluated once, independent of call order. */

#include <stddef.h>                 /* size_t */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/radical.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *g_cache = NULL;   /* dynamic array of found terms (k values) */
static size_t g_len   = 0;       /* number of terms stored */
static size_t g_cap   = 0;       /* current capacity of g_cache */
static idx_t  g_next  = 1;       /* next integer to examine */

/* Extend the cache until it contains at least n terms.  */
static void
ensure_cache(size_t n)
{
    while (g_len < n) {
        idx_t k = g_next++;
        seq_t radk = radical(k);
        seq_t val  = (seq_t)k - radk + 1;
        if (is_prime_big(val))
            append_candidate(&g_cache, &g_len, &g_cap, (seq_t)k);
    }
}

/* Public function: return the n‑th term (1‑based index).  */
seq_t
a(idx_t n)
{
    if (n == 0)                    /* offset is 1; undefined for 0 */
        return 0;
    if (g_len < (size_t)n)         /* compute more terms if necessary */
        ensure_cache((size_t)n);
    return g_cache[(size_t)n - 1];
}
