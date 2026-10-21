/*
 * A256225  Number of partitions of 5n into 5 parts.
 * https://oeis.org/A256225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of 5*n into exactly 5 parts
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 374, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#  include <new>
#endif

/* Allocate an array of seq_t of given size, zero‑initialized.
   Works for both C simple integer types and C++ big integer classes. */
static seq_t* alloc_seq_arr(size_t sz) {
#ifdef __cplusplus
    try {
        return new seq_t[sz]();
    } catch (...) {
        exit(1);
    }
#else
    return (seq_t*)calloc(sz, sizeof(seq_t));
#endif
}

/* Free an array allocated by alloc_seq_arr. */
static void free_seq_arr(seq_t* p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* a(n) = number of partitions of 5*n into exactly 5 parts. */
seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0) {
        return (seq_t)0;
    }

    /* target = 5*n - 5  (after subtracting 1 from each part) */
    idx_t target = 5 * n - 5;
    size_t sz = (size_t)target + 1;

    seq_t* dp = alloc_seq_arr(sz);          /* dp[0..target] */
    dp[0] = (seq_t)1;                      /* one partition of 0 */

    /* partitions into parts <= 5 */
    for (idx_t part = 1; part <= 5; ++part) {
        for (idx_t j = part; j <= target; ++j) {
            dp[j] += dp[j - part];
        }
    }

    seq_t result = dp[target];
    free_seq_arr(dp);
    return result;
}
