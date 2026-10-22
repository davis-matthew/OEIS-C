/*
 * A375955  T(n,k) for n >= 1, k <= n is the maximum value in the intersection
 *          of the Collatz trajectories of n and k, where a trajectory ends
 *          when it reaches 1. T(n,k) is a triangle read by rows.
 * https://oeis.org/A375955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the maximum common value in the Collatz trajectories of
 *            the * two integers that identify the n-th entry of the triangle
 *            T(r,k)
 * Range:     u8 n <= 377, u16 n <= 9605, u32 n <= 9605, u64 n <= 9605,
 *            u128 n <= 9605, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Require compiler to provide the types seq_t and idx_t. */
#include <stdlib.h>
#include <stdint.h>

static idx_t **traj_cache = NULL;   /* cached Collatz trajectories */
static size_t *traj_len   = NULL;
static size_t traj_cap    = 0;

static void ensure_traj_cache(idx_t n)
{
    if (n + 1 > traj_cap) {
        size_t old = traj_cap;
        traj_cap = (n + 1) * 2;
        traj_cache = (idx_t **)realloc(traj_cache, traj_cap * sizeof(idx_t *));
        traj_len   = (size_t *)realloc(traj_len,   traj_cap * sizeof(size_t));
        for (idx_t i = old; i < traj_cap; ++i) {
            traj_cache[i] = NULL;
            traj_len[i] = 0;
        }
    }
}

static idx_t *get_seq(idx_t n)
{
    ensure_traj_cache(n);
    if (traj_cache[n]) return traj_cache[n];

    size_t cap = 8;
    idx_t *arr = (idx_t *)malloc(cap * sizeof(idx_t));
    size_t len = 0;
    idx_t cur = n;

    while (1) {
        if (len == cap) {
            cap <<= 1;
            arr = (idx_t *)realloc(arr, cap * sizeof(idx_t));
        }
        arr[len++] = cur;
        if (cur == 1) break;
        if ((cur & 1) == 0) cur >>= 1;
        else cur = cur * 3 + 1;
    }

    arr = (idx_t *)realloc(arr, len * sizeof(idx_t));
    traj_cache[n] = arr;
    traj_len[n]   = len;
    return arr;
}

/* Triangle cache: row r holds r+1 entries (index 0 unused). */
static idx_t **tri_cache = NULL;
static size_t tri_cap    = 0;

static void ensure_tri_cache(idx_t r)
{
    if (r + 1 > tri_cap) {
        size_t old = tri_cap;
        tri_cap = (r + 1) * 2;
        tri_cache = (idx_t **)realloc(tri_cache, tri_cap * sizeof(idx_t *));
        for (idx_t i = old; i < tri_cap; ++i) tri_cache[i] = NULL;
    }
}

static idx_t compute_T(idx_t r, idx_t k)
{
    idx_t *traj_r = get_seq(r);
    idx_t *traj_k = get_seq(k);
    size_t len_r = traj_len[r];
    size_t len_k = traj_len[k];
    idx_t best = 0;

    for (size_t i = 0; i < len_k; ++i) {
        idx_t v = traj_k[i];
        for (size_t j = 0; j < len_r; ++j) {
            if (traj_r[j] == v) {
                if (v > best) best = v;
                break;
            }
        }
    }
    return best;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined; safe return */

    /* Find row r such that r(r+1)/2 >= n. */
    idx_t low = 1, high = 1;
    while (((unsigned __int128)high * (high + 1)) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t k = n - ((idx_t)(((unsigned __int128)(r - 1) * r) / 2));

    ensure_tri_cache(r);
    if (!tri_cache[r]) tri_cache[r] = (idx_t *)calloc(r + 1, sizeof(idx_t));

    if (tri_cache[r][k] == 0)          /* 0 signals "not yet computed" */
        tri_cache[r][k] = compute_T(r, k);

    return (seq_t)tri_cache[r][k];
}
