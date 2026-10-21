/*
 * A242101  Number of conjugacy classes of the symmetric group S_n when
 *          conjugating only by even permutations.
 * https://oeis.org/A242101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n (p(n)) + number of partitions
 *            of n into distinct odd parts (d(n)), for n >= 2; a(1) = 1
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) = p(n) + d(n) where
    - p(n) is the partition number of n,
    - d(n) counts partitions of n into distinct odd parts.   */
#include <stddef.h>
#include <stdlib.h>

static seq_t *p_arr = NULL;   /* partition numbers up to current_max_n */
static seq_t *d_arr = NULL;   /* distinct odd‐part partitions up to current_max_n */
static idx_t  current_max_n = 0;  /* highest n for which arrays are valid */

/* Helper: re-compute p_arr and d_arr up to new_max_n.  */
static void recompute_arrays(idx_t new_max_n)
{
    /* free previous arrays */
    free(p_arr);
    free(d_arr);

    /* allocate new arrays of size new_max_n + 1 */
    size_t sz = (size_t)(new_max_n + 1);
    p_arr = (seq_t *)calloc(sz, sizeof(seq_t));
    d_arr = (seq_t *)calloc(sz, sizeof(seq_t));

    /* ---------------------------------- */
    /* 1. Compute partition numbers p[n]  */
    /*    DP: for each part size k, add to all sums s >= k           */
    p_arr[0] = (seq_t)1;
    for (idx_t k = 1; k <= new_max_n; ++k) {
        for (idx_t s = k; s <= new_max_n; ++s) {
            p_arr[s] += p_arr[s - k];
        }
    }

    /* ---------------------------------- */
    /* 2. Compute partitions into distinct odd parts d[n]             */
    /*    For each odd k, update sums s from new_max_n down to k       */
    d_arr[0] = (seq_t)1;
    for (idx_t k = 1; k <= new_max_n; k += 2) {
        for (idx_t s = new_max_n; s >= k; --s) {
            d_arr[s] += d_arr[s - k];
        }
    }

    current_max_n = new_max_n;
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    if (n > current_max_n) {
        recompute_arrays(n);
    }

    /* Add the two precomputed values.             */
    /* Since seq_t is unsigned or a big‐int with
       native operators, the addition is exact.   */
    return p_arr[n] + d_arr[n];
}
