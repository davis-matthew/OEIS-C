/*
 * A387021  Number of permutations (p(1),p(2),...,p(n)) of (1,2,...,n) such
 *          that p(i)-i is in {-2,0,7} for all i=1,...,n.
 * https://oeis.org/A387021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 46, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n) = a(n-1) + 4*a(n-9) - 3*a(n-10) + 3*a(n-11)
                    - 2*a(n-12) + 2*a(n-13) - a(n-14) + a(n-15)
                    - 6*a(n-18) + 3*a(n-19) - 6*a(n-20) + 2*a(n-21)
                    - 3*a(n-22) + 4*a(n-27) - a(n-28) + 3*a(n-29) - a(n-36)
   with a(0)=1 and the first 35 terms computed by brute force. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Brute‑force counter for n < 36 */
static void dfs_brute(int pos, int n, bool *used, seq_t *res)
{
    if (pos > n) {
        (*res)++;
        return;
    }
    int cand;

    cand = pos - 2;
    if (cand >= 1 && !used[cand]) {
        used[cand] = true;
        dfs_brute(pos + 1, n, used, res);
        used[cand] = false;
    }

    cand = pos;
    if (!used[cand]) {
        used[cand] = true;
        dfs_brute(pos + 1, n, used, res);
        used[cand] = false;
    }

    cand = pos + 7;
    if (cand <= n && !used[cand]) {
        used[cand] = true;
        dfs_brute(pos + 1, n, used, res);
        used[cand] = false;
    }
}

static seq_t brute_count(idx_t n)
{
    bool used[40] = {0};          /* sufficient for n <= 35 */
    seq_t cnt = 0;
    if (n == 0) return 1;
    dfs_brute(1, (int)n, used, &cnt);
    return cnt;
}

static seq_t *cache = NULL;
static idx_t  cache_size = 0;
static idx_t  last_computed = (idx_t)-1;   /* no terms yet */

/* Ensure cache large enough to hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_size = n + 1;
        if (cache_size < 40) cache_size = 40;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        for (idx_t i = 0; i < cache_size; ++i) cache[i] = 0;
    } else if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }
}

static void compute_up_to(idx_t n)
{
    ensure_cache(n);
    idx_t start = (last_computed == (idx_t)-1) ? 0 : last_computed + 1;
    for (idx_t i = start; i <= n; ++i) {
        if (i < 36) {
            cache[i] = brute_count(i);
        } else {
            seq_t sum = cache[i-1];
            sum += (seq_t)4 * cache[i-9];
            sum -= (seq_t)3 * cache[i-10];
            sum += (seq_t)3 * cache[i-11];
            sum -= (seq_t)2 * cache[i-12];
            sum += (seq_t)2 * cache[i-13];
            sum -= cache[i-14];
            sum += cache[i-15];
            sum -= (seq_t)6 * cache[i-18];
            sum += (seq_t)3 * cache[i-19];
            sum -= (seq_t)6 * cache[i-20];
            sum += (seq_t)2 * cache[i-21];
            sum -= (seq_t)3 * cache[i-22];
            sum += (seq_t)4 * cache[i-27];
            sum -= cache[i-28];
            sum += (seq_t)3 * cache[i-29];
            sum -= cache[i-36];
            cache[i] = sum;
        }
        last_computed = i;
    }
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
