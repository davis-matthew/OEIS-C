/*
 * A251891  Number of (n+2) X (5+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not 2 3 6 or 7 and every diagonal and antidiagonal
 *          sum 2 3 6 or 7.
 * https://oeis.org/A251891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 26, u64 n <= 57, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical: a(n) = a(n-1) + 12*a(n-3) - 12*a(n-4) - 32*a(n-6) + 32*a(n-7)
   for n>12.  Initial values a(1)…a(12) are listed below. */
#ifdef __cplusplus
#include <algorithm>
#endif
#include <stddef.h>
#include <stdlib.h>
#ifndef __cplusplus
#include <string.h>
#endif

seq_t a(idx_t n) {
    /* offset starts at 1, ignore n==0 except for safety */
    if (n == 0) return 0;

    /* cast to a type suitable for array indices */
    size_t N = (size_t)n;

    /* static memoisation storage */
    static seq_t *memo   = NULL;
    static size_t memo_cap      = 0;   /* capacity of memo array   */
    static size_t memo_computed = 0;   /* highest index already computed */

    /* ensure we have enough room */
    if (N + 1 > memo_cap) {
        size_t new_cap = N + 1;
        seq_t *new_ptr;
#ifdef __cplusplus
        /* allocate a new array and copy previous contents */
        new_ptr = new seq_t[new_cap];
        if (memo) {
            std::copy(memo, memo + memo_cap, new_ptr);
            delete [] memo;
        }
#else
        /* use realloc – it copies the data automatically for C */
        new_ptr = realloc(memo, new_cap * sizeof(seq_t));
        if (!new_ptr) return 0;   /* allocation failure */
#endif
        memo = new_ptr;
        memo_cap = new_cap;
    }

    /* initial 12 terms (index 1 … 12) */
    static const seq_t init[13] = {
        0ULL,          /* unused for idx 0 */
        1534ULL, 594ULL, 1260ULL, 1958ULL, 3530ULL,
        6072ULL, 11762ULL, 21346ULL, 39202ULL,
        80834ULL, 147842ULL, 276162ULL
    };

    /* compute missing terms up to N */
    for (size_t i = memo_computed + 1; i <= N; ++i) {
        if (i <= 12) {
            memo[i] = init[i];
        } else {
            seq_t part1 = memo[i-1] + memo[i-3] * (seq_t)12;
            seq_t part2 = memo[i-4] * (seq_t)12 + memo[i-6] * (seq_t)32;
            seq_t diff  = part1 - part2;
            memo[i] = diff + memo[i-7] * (seq_t)32;
        }
    }

    memo_computed = N;
    return memo[N];
}
