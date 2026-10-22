/*
 * A319401  Number of partitions of n into exactly eight positive Fibonacci
 *          numbers.
 * https://oeis.org/A319401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   Product_{j>=2} 1/(1 - y * x^{F_j}) --> a(n) = number of
 *            partitions of n into exactly 8 positive Fibonacci numbers
 * Range:     u8 n <= 7491, u16 n <= 7491, u32 n <= 7491, u64 n <= 7491,
 *            u128 n <= 7491, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Conditional allocation/deallocation to work both in C and in C++ with arbitrary‑precision seq_t */
#if defined(__cplusplus)
#  define ALLOC_SEQ_ARRAY(len)  new seq_t[(len)]()
#  define FREE_SEQ_ARRAY(ptr)   delete[] (ptr)
#else
#  define ALLOC_SEQ_ARRAY(len)  ((seq_t*)calloc((len), sizeof(seq_t)))
#  define FREE_SEQ_ARRAY(ptr)   free((ptr))
#endif

seq_t a(idx_t n)
{
    /* Prepare Fibonacci numbers ≤ n (positive Fibonacci numbers start 1,2,3,5,...) */
    static const size_t MAX_FIBS = 64;
    idx_t fibs[MAX_FIBS];
    size_t fib_cnt = 0;

    idx_t a_fib = 1, b_fib = 2;   /* first two positive Fibonacci numbers after 0,1 */
    fibs[fib_cnt++] = a_fib;
    if (fib_cnt < MAX_FIBS) fibs[fib_cnt++] = b_fib;

    while (1) {
        idx_t c_fib = a_fib + b_fib;
        if (c_fib > n) break;
        if (fib_cnt < MAX_FIBS) fibs[fib_cnt++] = c_fib;
        a_fib = b_fib;
        b_fib = c_fib;
    }

    /* DP table: dp[part][sum]  part = 0…8, sum = 0…n */
    size_t width = (size_t)n + 1;
    seq_t *dp[9];
    for (size_t p = 0; p < 9; ++p) {
        dp[p] = ALLOC_SEQ_ARRAY(width);
        if (!dp[p]) {                   /* allocation failure → return 0 */
            for (size_t q = 0; q < p; ++q) FREE_SEQ_ARRAY(dp[q]);
            return (seq_t)0;
        }
    }
    dp[0][0] = (seq_t)1;                           /* one way to make sum 0 with 0 parts */

    /* Core DP: for each fibonacci “coin” update the tables */
    for (size_t fi = 0; fi < fib_cnt; ++fi) {
        idx_t f = fibs[fi];
        for (size_t part = 1; part <= 8; ++part) {
            for (idx_t sum = f; sum <= n; ++sum) {
                dp[part][sum] += dp[part-1][sum - f];
            }
        }
    }

    seq_t result = dp[8][n];

    /* Clean up */
    for (size_t p = 0; p < 9; ++p) FREE_SEQ_ARRAY(dp[p]);

    return result;
}
