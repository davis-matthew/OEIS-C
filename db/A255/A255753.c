/*
 * A255753  Number of (n+2)X(5+2) 0..1 arrays with every 3X3 subblock sum of
 *          the medians of the diagonal and antidiagonal minus the two sums of
 *          the central row and column nondecreasing horizontally, vertically
 *          and ne-to-sw antidiagonally.
 * https://oeis.org/A255753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 4*a(n-4) - 8*a(n-5) + 4*a(n-6) for n
 *            > 18 Initial values (n = 1 … 18): 99200, 180619, 197284, 203503,
 *            196744, 237248, 300504, 402972, 543216, 719217, 966484, 1367292,
 *            1914280, 2603820, 3577560, 5163800, 7335400, 10077720
 * Range:     u8 none, u16 none, u32 n <= 35, u64 n <= 99, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[18] = {
    99200ULL, 180619ULL, 197284ULL, 203503ULL, 196744ULL,
    237248ULL, 300504ULL, 402972ULL, 543216ULL, 719217ULL,
    966484ULL, 1367292ULL, 1914280ULL, 2603820ULL,
    3577560ULL, 5163800ULL, 7335400ULL, 10077720ULL
};

seq_t a(idx_t n)
{
    /* For the trivial cases return the hard‑coded initial values. */
    if (n == 0) return 0;            /* offset does not matter for n < 1 */
    if (n <= 18) return base_vals[n-1];

    static seq_t *memo = NULL;
    static idx_t memo_len = 0;   /* number of terms stored (>= 18) */
    static idx_t memo_cap = 0;   /* capacity of the allocation */

    /* Initialise memoisation storage on the first call. */
    if (memo_len == 0) {
        memo_cap = 64;             /* start with a modest capacity */
        memo      = (seq_t*)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return 0;       /* allocation failure – return zero */
        for (idx_t i = 0; i < 18; ++i) memo[i] = base_vals[i];
        memo_len = 18;
    }

    /* Grow the buffer if we will need more space. */
    if (n > memo_cap) {
        idx_t new_cap = memo_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Expand the sequence up to the requested index n. */
    for (idx_t i = memo_len + 1; i <= n; ++i) {
        seq_t s1 = 2 * memo[i-2];           /* 2*a(i-1) */
        seq_t s2 = memo[i-3];               /* a(i-2)  */
        seq_t s3 = 4 * memo[i-5];           /* 4*a(i-4)*/
        seq_t s4 = 8 * memo[i-6];           /* 8*a(i-5)*/
        seq_t s5 = 4 * memo[i-7];           /* 4*a(i-6)*/
        memo[i-1] = s1 - s2 + s3 - s4 + s5;
    }

    memo_len = n;
    return memo[n-1];
}
