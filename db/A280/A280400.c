/*
 * A280400  Number of 2Xn 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A280400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Sequence A280400 (OEIS)
 *  Recurrence: a(n) = 3*a(n-1) + a(n-2) - 7*a(n-3) + 4*a(n-5)
 *  Base values (n = 1…5): 0, 2, 6, 16, 34
 *  Offset: 1  (a(1) is the first term listed)
 *
 *  The implementation uses a static cache up to 10000 entries.
 *  It works for all supported seq_t / idx_t types, including
 *  arbitrary‑precision integer classes and primitive unsigned types.
 *  The cache array is statically dimensioned to avoid explicit
 *  C++ construction/destruction issues, which keeps the code valid
 *  both for C11 and C++17.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t if needed */

#define MAX_N 10000   /* maximum n expected by the tests */

/* static cache: index 0 is unused (to keep 1‑based addressing) */
static seq_t cache[MAX_N + 1];
/* cache_len holds the largest index that has been initialized (0 if none) */
static idx_t cache_len = 0;

/* Initialize the first five terms of the sequence. */
static void init_base(void) {
    if (cache_len < 5) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)16;
        cache[5] = (seq_t)34;
        cache_len = 5;
    }
}

/* Recursive sequence function */
seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;      /* out of range; not expected */

    if (n > MAX_N) {        /* safety guard: fall back to pure recursion */
        /* compute via iterative method without caching beyond MAX_N */
        seq_t prev5 = 0, prev4 = 0, prev3 = 0, prev2 = 0, prev1 = 0;
        seq_t cur;
        /* initialise the first five terms */
        prev5 = 0;          /* a(0) is not used but needed for placeholder */
        prev4 = 0;          /* will be overwritten by a(1) */
        prev3 = 0;          /* will be overwritten by a(2) */
        prev2 = 0;          /* will be overwritten by a(3) */
        prev1 = 0;          /* will be overwritten by a(4) */
        /* a(1) … a(5) */
        prev4 = (seq_t)0;
        prev3 = (seq_t)2;
        prev2 = (seq_t)6;
        prev1 = (seq_t)16;
        cur    = (seq_t)34;   /* a(5) */

        if (n == 5) return cur;
        /* compute subsequent terms */
        for (idx_t i = 6; i <= n; ++i) {
            prev5 = prev4;
            prev4 = prev3;
            prev3 = prev2;
            prev2 = prev1;
            prev1 = cur;
            cur = 3 * prev1 + prev2 - 7 * prev3 + 4 * prev5;
        }
        return cur;
    }

    /* Ensure base values are set */
    init_base();

    if (n <= cache_len)
        return cache[n];    /* already computed */

    /* compute from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 5) {
            /* base values already correctly set */
            continue;
        }
        seq_t val = 3 * cache[i - 1] + cache[i - 2]
                    - 7 * cache[i - 3] + 4 * cache[i - 5];
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}
