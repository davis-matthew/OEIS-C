/*
 * A256010  Product of n and the total number of parts in all partitions of n.
 *          Also, product of n and the sum of largest parts of all partitions
 *          of n.
 * https://oeis.org/A256010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A006128(n) where A006128(n) is the sum of the largest
 *            parts of all partitions of n. The implementation recomputes the
 *            partition numbers, the divisor function and their convolution
 *            for up to the current maximum index. This works for all seq_t
 *            sizes because only integer arithmetic is used
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External types supplied by the build */
typedef uint64_t idx_t;
/* seq_t is defined in the build environment; no definition here */

static seq_t *p_arr = NULL;      /* partition numbers p[n]   */
static seq_t *s_arr = NULL;      /* largest-part sums s[n]   */
static uint32_t *d_arr = NULL;   /* divisor counts d[n]     */
static idx_t computed_max = 0;   /* largest n that has been computed */

/* Resize arrays and recompute all values up to n */
static void ensure_capacity(idx_t n)
{
    if (n <= computed_max) return;

    /* Allocate new storage */
    seq_t *new_p = (seq_t *)realloc(p_arr, (n + 1) * sizeof(seq_t));
    seq_t *new_s = (seq_t *)realloc(s_arr, (n + 1) * sizeof(seq_t));
    uint32_t *new_d = (uint32_t *)realloc(d_arr, (n + 1) * sizeof(uint32_t));
    if (!new_p || !new_s || !new_d) abort();

    p_arr = new_p; s_arr = new_s; d_arr = new_d;

    /* Zero all entries (including the new ones) */
    for (idx_t i = 0; i <= n; ++i) {
        p_arr[i] = (seq_t)0;
        s_arr[i] = (seq_t)0;
        d_arr[i] = (uint32_t)0;
    }

    /* Partition numbers via simple O(n^2) DP */
    p_arr[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t s = i; s <= n; ++s) {
            p_arr[s] += p_arr[s - i];
        }
    }

    /* Divisor function d[n] */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            ++d_arr[j];
        }
    }

    /* Convolution s[n] = sum_{j=1}^n d[j] * p[n-j] */
    for (idx_t j = 1; j <= n; ++j) {
        seq_t dj = (seq_t)d_arr[j];
        for (idx_t s = j; s <= n; ++s) {
            s_arr[s] += dj * p_arr[s - j];
        }
    }

    computed_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0;
    ensure_capacity(n);
    return (seq_t)n * s_arr[n];
}
