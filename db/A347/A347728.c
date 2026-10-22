/*
 * A347728  Number of compositions (ordered partitions) of n into at most 5
 *          cubes.
 * https://oeis.org/A347728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into at most 5 cubes
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The array dp[k][s] counts compositions of sum s using exactly k parts
 * that are perfect cubes (1, 8, 27, …).  The final answer is the sum over
 * k = 0..5 of dp[k][n].  dp[0][0] is 1 (empty composition), all other
 * dp[0][s>0] are 0.  The table is constructed incrementally to support
 * calls in any order.  All arithmetic uses only the permitted operators
 * and types.  This implementation works for any seq_t and idx_t
 * definitions provided by the build. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
/* The build's seq_t is a default‑constructible big integer class.
 * We use operator new[] / delete[] in C++ mode and malloc / free in C. */
static seq_t* alloc_arr(size_t size)
{
    if (size == 0) return NULL;
    return new seq_t[size]();
}
static seq_t* resize_arr(seq_t *old, size_t old_size, size_t new_size)
{
    if (!old) return alloc_arr(new_size);
    seq_t *res = new seq_t[new_size]();
    size_t copy = old_size < new_size ? old_size : new_size;
    for (size_t i = 0; i < copy; ++i) res[i] = old[i];
    delete[] old;
    return res;
}
#else
static seq_t* alloc_arr(size_t size)
{
    if (size == 0) return NULL;
    seq_t *p = (seq_t*)malloc(sizeof(seq_t)*size);
    if (p) memset(p,0,sizeof(seq_t)*size);
    return p;
}
static seq_t* resize_arr(seq_t *old, size_t old_size, size_t new_size)
{
    if (!old) return alloc_arr(new_size);
    seq_t *res = (seq_t*)realloc(old,sizeof(seq_t)*new_size);
    if (res && new_size>old_size) memset(res+old_size,0,sizeof(seq_t)*(new_size-old_size));
    return res;
}
#endif

/* Memoisation state */
static seq_t *dp[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
static idx_t  max_cached = (idx_t)(-1);

/* Build DP table up to `limit`. */
static void compute_dp(idx_t limit)
{
    if (max_cached == (idx_t)(-1)) {
        for (int k=0;k<=5;++k) dp[k] = alloc_arr(1);
        dp[0][0] = 1;            /* empty composition for sum 0 */
        for (int k=1;k<=5;++k) dp[k][0] = 0;
        max_cached = 0;
    }
    if (limit <= max_cached) return;

    size_t old_size = (size_t)max_cached + 1;
    size_t new_size = (size_t)limit + 1;
    for (int k=0;k<=5;++k) dp[k] = resize_arr(dp[k],old_size,new_size);

    for (idx_t s = old_size; s <= limit; ++s) {
        for (idx_t c = 1; ; ++c) {
            idx_t cube = c*c*c;
            if (cube > s) break;
            for (int k = 1; k <= 5; ++k)
                dp[k][s] = dp[k][s] + dp[k-1][s-cube];
        }
    }
    max_cached = limit;
}

/* Required sequence function. */
seq_t a(idx_t n)
{
    compute_dp(n);
    seq_t total = dp[0][n];
    for (int k=1; k<=5; ++k) total = total + dp[k][n];
    return total;
}
