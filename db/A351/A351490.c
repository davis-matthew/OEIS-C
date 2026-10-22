/*
 * A351490  Irregular triangle read by rows: T(n,k) is the minimum number of
 *          alphabetic symbols in a regular expression for the k
 *          lexicographically first palindromes of odd length 2*n-1 over a
 *          binary alphabet, n >= 1, 1 <= k <= 2^n.
 * https://oeis.org/A351490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) with r = floor(log2(n+1)) and * k = n - (2^r - 1)
 *            + 1 * T(n,k) = 2*n + 3*(k-1) - 2*popcount(k-1) - 1 * This
 *            implements A351490 (flattened irregular triangle)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* sequence starts at n = 1 */
    if (n == 0) return 0;

    /* Convert to 64‑bit unsigned for bit operations */
    uint64_t nval = (uint64_t)n;
    uint64_t np1  = nval + 1;                /* n+1 >= 2 */
    uint64_t r    = 63 - __builtin_clzll(np1); /* floor(log2(n+1)) */

    /* start index of row r (1‑based) */
    uint64_t start = (1ull << r) - 1;         /* 2^r - 1  */
    uint64_t k     = nval - start + 1;       /* 1‑based in row */

    /* Hamming weight of k-1 (fits in 64 bits for all test data up to 10000) */
    uint64_t km1   = k - 1;
    uint64_t weight = __builtin_popcountll(km1);

    /* Compute the term using seq_t arithmetic */
    seq_t row = (seq_t)r;
    seq_t term = 2 * row + 3 * (seq_t)km1 - 2 * (seq_t)weight - 1;
    return term;
}
