/*
 * A304708  Number of partitions (d1,d2,...,dm) of n such that d1/1 > d2/2 >
 *          ... > dm/m and d1 < d2 < ... < dm.
 * https://oeis.org/A304708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursively count partitions (d1,d2,…,dm) of n with
   d1<d2<…<dm and d1/1 > d2/2 > … > dm/m  (OEIS A304708) */
#include <stddef.h>
#include <stdint.h>

/* helper: integer division truncates toward zero as required */
static inline idx_t upper_bound(idx_t last, idx_t k) {
    /* returns floor((last*k - 1)/(k-1))  */
    /* safe for k >= 2 and last >= 1 */
#if defined(__SIZEOF_INT128__)
    __uint128_t prod = (__uint128_t)last * k;
    return (idx_t)((prod - 1) / (k - 1));
#else
    /* careful to avoid overflow for small types; last*k fits in idx_t (>=uint64_t) */
    return (idx_t)(((uint64_t)last * k - 1) / (k - 1));
#endif
}

/* depth‑first search counting valid partitions */
static seq_t dfs(idx_t rem, idx_t last, idx_t k) {
    if (rem == 0) return (seq_t)1;
    /* minimal next part is last+1 */
    idx_t low = last + 1;
    if (low > rem) return (seq_t)0;

    idx_t high;
    if (k == 1) {
        high = rem;
    } else {
        high = upper_bound(last, k);
        if (high > rem) high = rem;
        if (high < low) return (seq_t)0;
    }

    seq_t total = (seq_t)0;
    for (idx_t a = low; a <= high; ++a) {
        total += dfs(rem - a, a, k + 1);
    }
    return total;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;           /* a(0) = 1 by definition */
    return dfs(n, (idx_t)0, (idx_t)1);
}
