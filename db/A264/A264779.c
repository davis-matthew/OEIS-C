/*
 * A264779  Numbers n for which the numbers 6n+1, 3n+2, 6n+7 are all odd
 *          composite squarefree, but not all semiprimes.
 * https://oeis.org/A264779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that 6m+1, 3m+2, 6m+7 are all odd
 *            composite squarefree numbers, and not all three are semiprimes
 * Range:     u8 none, u16 n <= 774, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 63 terms through n = 1573 (2026-11-03)
 * Helpers:   is_prime_u64, is_squarefree, mem, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/omega.h"
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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static bool good_candidate(idx_t m) {
    const seq_t a1 = ((seq_t)6 * m) + 1;
    const seq_t a2 = ((seq_t)3 * m) + 2;
    const seq_t a3 = ((seq_t)6 * m) + 7;

    /* 3m+2 must be odd; reject even m */
    if ((m & 1U) == 0)
        return false;

    const seq_t vals[3] = { a1, a2, a3 };
    bool all_semiprime = true;
    for (int i = 0; i < 3; ++i) {
        seq_t x = vals[i];
        /* must be odd, composite, and squarefree */
        if ((x & ((seq_t)1)) == 0 || x <= (seq_t)1 || is_prime_big(x))
            return false;
        if (!is_squarefree(x))
            return false;
        /* semiprime test: squarefree number with exactly two prime factors */
        uint64_t ux = (uint64_t)x;          /* fits in 64 bits for the required range */
        int pcount = omega(ux);
        if (pcount != 2)
            all_semiprime = false;
    }
    return !all_semiprime;   /* at least one must not be a semiprime */
}

static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  last_tested = 0;

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;      /* undefined for n=0 – harmless */

    if (cache_len < (size_t)n) {
        size_t needed = n;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed)
                new_cap <<= 1;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp)
                return (seq_t)0;   /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t m = last_tested + 1; ; ++m) {
            if ((m & 1U) == 0)
                continue;   /* 3m+2 would be even */
            if (good_candidate(m)) {
                cache[cache_len++] = (seq_t)m;
                if (cache_len >= (size_t)n) {
                    last_tested = m;
                    break;
                }
            }
            last_tested = m;
        }
    }

    return cache[n - 1];
}
