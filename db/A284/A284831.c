/*
 * A284831  Expansion of Sum_{i>=1} x^(i^3)/(1 - x^(i^3)) * Product_{j>=i}
 *          1/(1 - x^(j^3)).
 * https://oeis.org/A284831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of smallest parts in all partitions of n
 *            into cubes (A284831) Computation: For each cube c ≤ n, build
 *            partitions using cubes ≥ c and sum the multiplicity of c as the
 *            smallest part
 * Range:     u8 n <= 52, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static struct {
    idx_t  maxN;   /* largest n already computed */
    seq_t *a;      /* cached sequence values up to maxN */
} cache = {0, NULL};

/* Re‑compute sequence up to new_max (must be > cache.maxN) */
static void recompute(idx_t new_max)
{
    /* 1. list all cube values ≤ new_max */
    const idx_t MAX_CUBES = 1024;   /* safe upper bound for 64‑bit cubes */
    idx_t *cubes = (idx_t*)malloc(MAX_CUBES * sizeof(idx_t));
    size_t cube_cnt = 0;
    for (idx_t i = 1;; ++i) {
        /* use 128‑bit to avoid overflow on i*i*i */
        __int128 sq = (__int128)i * i * i;
        if (sq > ( (__int128)new_max )) break;
        cubes[cube_cnt++] = (idx_t)sq;
    }

    /* 2. allocate working arrays */
    const size_t sz = (size_t)new_max + 1;
    seq_t *result = (seq_t*)calloc(sz, sizeof(seq_t));   /* a[n] values */
    seq_t *count  = (seq_t*)calloc(sz, sizeof(seq_t));   /* number of partitions */
    seq_t *sum    = (seq_t*)calloc(sz, sizeof(seq_t));   /* sum of multiplicities of current c */

    /* 3. for each cube c, compute sum of multiplicities of c as smallest part */
    for (size_t idx_c = 0; idx_c < cube_cnt; ++idx_c) {
        idx_t c_val = cubes[idx_c];

        /* reset count and sum arrays */
        memset(count, 0, sz * sizeof(seq_t));
        memset(sum,   0, sz * sizeof(seq_t));
        count[0] = (seq_t)1;    /* base: one way to make 0 */

        /* process all cubes from c upward */
        for (size_t idx_val = idx_c; idx_val < cube_cnt; ++idx_val) {
            idx_t val = cubes[idx_val];
            for (size_t amt = val; amt <= new_max; ++amt) {
                seq_t addCnt = count[amt - val];
                seq_t addSum;
                if (val == c_val) {
                    addSum = sum[amt - val] + addCnt;  /* each new copy of c adds one */
                } else {
                    addSum = sum[amt - val];
                }
                count[amt] = count[amt] + addCnt;
                sum[amt]   = sum[amt]   + addSum;
            }
        }

        /* accumulate result[n] += sum[n] for all n */
        for (idx_t n = 1; n <= new_max; ++n)
            result[n] = result[n] + sum[n];
    }

    /* 4. free old cache, store new data */
    if (cache.a != NULL) free(cache.a);
    cache.a   = result;
    cache.maxN = new_max;

    free(count);
    free(sum);
    free(cubes);
}

/* Public function: returns a(n) for any n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n > cache.maxN) recompute(n);
    return cache.a[n];
}
