/*
 * A350726  a(n) = Sum_{k=0..n} k^(n-k) * Stirling1(n,k).
 * https://oeis.org/A350726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} k^(n-k) * Stirling1(n,k)
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 18,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static seq_t *powk = NULL;
static idx_t powk_cap = 0;
static seq_t *prev = NULL;
static idx_t prev_cap = 0;
static seq_t *cur = NULL;
static idx_t cur_cap = 0;
static idx_t max_n_done = 0;
static int initialized = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    seq_t *arr = seq_alloc_new_array(n + 1);
    if (cache) {
        for (idx_t i = 0; i < cache_cap; ++i) arr[i] = cache[i];
        seq_free_array(cache);
    }
    cache = arr;
    cache_cap = n + 1;
}

static void ensure_powk(idx_t n)
{
    if (n < powk_cap) return;
    seq_t *arr = seq_alloc_new_array(n + 1);
    if (powk) {
        for (idx_t i = 0; i < powk_cap; ++i) arr[i] = powk[i];
        seq_free_array(powk);
    }
    powk = arr;
    powk_cap = n + 1;
}

static void ensure_prev_cur(idx_t n)
{
    idx_t need = n + 1;
    if (need <= prev_cap && need <= cur_cap) return;
    seq_t *arr_prev = seq_alloc_new_array(need);
    seq_t *arr_cur  = seq_alloc_new_array(need);
    if (prev) {
        for (idx_t i = 0; i < prev_cap; ++i) arr_prev[i] = prev[i];
        seq_free_array(prev);
    }
    if (cur) {
        for (idx_t i = 0; i < cur_cap; ++i) arr_cur[i] = cur[i];
        seq_free_array(cur);
    }
    prev = arr_prev;   prev_cap = need;
    cur  = arr_cur;    cur_cap = need;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        cache  = seq_alloc_new_array(1);  cache[0] = 1;  cache_cap = 1;
        powk   = seq_alloc_new_array(1);  powk[0]  = 1;  powk_cap  = 1;
        prev   = seq_alloc_new_array(1);  prev[0]  = 1;  prev_cap  = 1;
        cur    = seq_alloc_new_array(1);  cur_cap   = 1;
        max_n_done = 0;
        initialized = 1;
        if (n == 0) return 1;
    }

    if (n <= max_n_done) return cache[n];

    for (idx_t m = max_n_done + 1; m <= n; ++m) {
        ensure_powk(m);
        if (m > 0) {
            for (idx_t k = 0; k < m; ++k) {
                powk[k] = powk[k] * (seq_t)k;
            }
        }
        powk[m] = 1;          /* k^0 for k=m */

        ensure_prev_cur(m);

        cur[0] = 0;           /* s(m,0) = 0 for m>0 */
        for (idx_t k = 1; k < m; ++k) {
            cur[k] = prev[k-1] - (seq_t)(m-1) * prev[k];
        }
        cur[m] = 1;           /* s(m,m) = 1 */

        seq_t sum = 0;
        for (idx_t k = 0; k <= m; ++k) {
            sum += cur[k] * powk[k];
        }
        ensure_cache(m);
        cache[m] = sum;

        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }
    max_n_done = n;
    return cache[n];
}
