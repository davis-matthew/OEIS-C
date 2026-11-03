/*
 * A262838  {3,7}-primes (defined in Comments).
 * https://oeis.org/A262838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 982 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th {3,7}-prime: a prime p such that the digits of p in base
   3 spell a prime in base 7 and vice versa.  The sequence is stored in a
   dynamic cache for repeated calls. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Reinterpret the digits of a number written in base 'from'
   as a number written in base 'to'. */
static inline seq_t convert_base(seq_t value, unsigned from, unsigned to)
{
    seq_t digits[128];
    size_t len = 0;
    seq_t v = value;

    if (v == 0) return 0;
    while (v > 0) {
        digits[len++] = v % from;
        v = v / from;
    }

    seq_t res = 0;
    for (size_t i = len; i > 0; --i) {
        res = res * to + digits[i - 1];
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_len = 0;
    static idx_t   next_candidate = 2;

    while (cache_len < n) {
        /* Find next prime candidate. */
        seq_t p;
        do {
            p = (seq_t)next_candidate;
            ++next_candidate;
        } while (!is_prime_big(p));

        /* Check base-3 → base-7 conversion. */
        seq_t v3to7 = convert_base(p, 3, 7);
        if (!is_prime_big(v3to7))
            continue;

        /* Check base-7 → base-3 conversion. */
        seq_t v7to3 = convert_base(p, 7, 3);
        if (!is_prime_big(v7to3))
            continue;

        /* Store the {3,7}-prime. */
        size_t needed = cache_len + 1;
        ensure_capacity(needed, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[cache_len++] = p;
    }

    return cache[n - 1];
}
