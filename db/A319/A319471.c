/*
 * A319471  Number of partitions of n into exactly six nonzero decimal
 *          palindromes.
 * https://oeis.org/A319471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 910, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of n into exactly six nonzero
 * decimal palindromes (order of parts does not matter).
 *
 * Implementation: generate all decimal palindromes up to n,
 * then use DP to count partitions with exactly six parts.
 * Time:   O(#palindromes * 6 * n)   (≤ 200 * 6 * 10000 ≪ 2 s)
 * Memory: O(7 * (n+1)) elements.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_PALS 200    /* sufficient for all palindromes <= 10000 */

/* generate all decimal palindromes ≤ limit into pals[0..] and set palcnt */
static void generate_palindromes_upto(idx_t limit, idx_t pals[MAX_PALS], size_t *palcnt)
{
    size_t sz = 0;

    /* 1‑digit palindromes 1–9 */
    for (int d = 1; d <= 9 && (idx_t)d <= limit; ++d)
        pals[sz++] = (idx_t)d;

    /* 2‑digit palindromes aa */
    for (int a = 1; a <= 9; ++a) {
        idx_t val = a * 10 + a;
        if (val > limit) break;
        pals[sz++] = val;
    }

    /* 3‑digit palindromes aba */
    for (int a = 1; a <= 9; ++a)
        for (int b = 0; b <= 9; ++b) {
            idx_t val = a * 100 + b * 10 + a;
            if (val > limit) break;
            pals[sz++] = val;
        }

    /* 4‑digit palindromes abba */
    for (int a = 1; a <= 9; ++a)
        for (int b = 0; b <= 9; ++b) {
            idx_t val = a * 1000 + b * 100 + b * 10 + a;
            if (val > limit) break;
            pals[sz++] = val;
        }

    *palcnt = sz;
}

seq_t a(idx_t n)
{
    /* collect all palindromes up to n */
    idx_t pals[MAX_PALS];
    size_t palcnt;
    generate_palindromes_upto(n, pals, &palcnt);

    /* DP table: dp[k][s] flattened as k*cols + s */
    size_t rows = 7;                 /* indices 0..6 */
    size_t cols = (size_t)(n + 1);   /* sums 0..n */
    size_t total = rows * cols;
    seq_t *dp;

#ifdef __cplusplus
    dp = new seq_t[total];
    for (size_t i = 0; i < total; ++i) dp[i] = 0;
#else
    dp = (seq_t *)calloc(total, sizeof(seq_t));
#endif

    /* base case: one way to form 0 with 0 parts */
    dp[0] = (seq_t)1;

    /* DP transition for each palindrome */
    for (size_t pi = 0; pi < palcnt; ++pi) {
        idx_t p = pals[pi];
        for (int k = 1; k <= 6; ++k) {
            for (idx_t s = p; s <= n; ++s) {
                size_t idx  = (size_t)k * cols + (size_t)s;
                size_t sub  = (size_t)(k - 1) * cols + (size_t)(s - p);
                dp[idx] += dp[sub];
            }
        }
    }

    seq_t result = dp[(size_t)6 * cols + (size_t)n];

#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
