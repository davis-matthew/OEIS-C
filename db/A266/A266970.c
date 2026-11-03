/*
 * A266970  Primes of the form x^3 + x*y + y^3 where x and y are positive
 *          integers.
 * https://oeis.org/A266970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   No closed form—search all values of x^3 + x*y + y^3, keep the
 *            primes in sorted increasing order
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include required utilities */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

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

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* allocator/deallocator that works in both C and C++ */
#ifdef __cplusplus
static seq_t* allocate_seq(size_t cap) { return new seq_t[cap]; }
static void free_seq_arr(seq_t *ptr) { delete[] ptr; }
#else
static seq_t* allocate_seq(size_t cap) { return (seq_t*)malloc(cap * sizeof(seq_t)); }
static void free_seq_arr(seq_t *ptr) { free(ptr); }
#endif

/* dynamic array growth helper for seq_t */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap > 0) ? (*cap << 1) : 64;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = allocate_seq(newcap);
        for (size_t i = 0; i < *cap; ++i) tmp[i] = (*arr)[i];
        free_seq_arr(*arr);
        *arr = tmp;
        *cap = newcap;
    }
}

/* generate all primes of the required form <= limit */
static void build_primes_upto(seq_t limit, seq_t **out, size_t *cnt)
{
    seq_t *candidates = NULL;
    size_t cand_cap = 0, cand_cnt = 0;

    seq_t max_x = floor_cube_root(limit);
    for (idx_t xi = 1; xi <= (idx_t)max_x; ++xi) {
        seq_t x = (seq_t)xi;
        seq_t x3 = x * x * x;
        if (x3 > limit) break;
        seq_t rem = limit - x3;
        if (rem < 1) continue;

        seq_t y_max = floor_cube_root(rem);
        if (y_max > x) y_max = x;

        for (idx_t yi = 1; yi <= (idx_t)y_max; ++yi) {
            seq_t y = (seq_t)yi;
            seq_t val = x3 + x * y + y * y * y;
            if (val > limit) continue;
            grow_seq(&candidates, &cand_cap, cand_cnt + 1);
            candidates[cand_cnt++] = val;
        }
    }

    if (cand_cnt == 0) {
        *out = NULL;
        *cnt = 0;
        free_seq_arr(candidates);
        return;
    }

    if (cand_cnt > 1) mem_qsort(candidates, cand_cnt, sizeof(seq_t), cmp_seq_t);

    seq_t *primes = NULL;
    size_t primes_cap = 0, primes_cnt = 0;
    for (size_t i = 0; i < cand_cnt; ++i) {
        if (i == 0 || candidates[i] != candidates[i-1]) {
            if (is_prime_big(candidates[i])) {
                grow_seq(&primes, &primes_cap, primes_cnt + 1);
                primes[primes_cnt++] = candidates[i];
            }
        }
    }

    free_seq_arr(candidates);
    *out = primes;
    *cnt = primes_cnt;
}

/* static cache for primes of the form, in increasing order */
static seq_t *prime_cache = NULL;
static size_t prime_cnt   = 0;
static seq_t cache_limit  = 0;

/* main sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset < 1 */

    while (prime_cnt < (size_t)n) {
        seq_t new_limit = (cache_limit == 0) ? (seq_t)3 : cache_limit * 2;

        seq_t *tmp = NULL;
        size_t tmpcnt = 0;
        build_primes_upto(new_limit, &tmp, &tmpcnt);

        free_seq_arr(prime_cache);
        prime_cache = tmp;
        prime_cnt   = tmpcnt;
        cache_limit = new_limit;
    }
    return prime_cache[n-1];
}
