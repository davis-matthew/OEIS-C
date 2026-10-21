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
 * Helpers:   cmp_seq_t, floor_cube_root, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include required utilities */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/cmp_seq_t.h"
#include "../utils/is_prime_big.h"
#include "../utils/floor_cube_root.h"

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

    if (cand_cnt > 1) qsort(candidates, cand_cnt, sizeof(seq_t), cmp_seq_t);

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
