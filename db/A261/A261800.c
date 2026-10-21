/*
 * A261800  Number of 8-compositions of n: matrices with 8 rows of nonnegative
 *          integers with positive column sums and total element sum n.
 * https://oeis.org/A261800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16 a(n‑1) – 56 a(n‑2) + 112 a(n‑3) – 140 a(n‑4) + 112
 *            a(n‑5) – 56 a(n‑6) + 16 a(n‑7) – 2 a(n‑8) for n>8 Initial values
 *            (OEIS offset 0): a(0)=1, a(1)=8, a(2)=100, a(3)=1208,
 *            a(4)=14554, a(5)=175352, a(6)=2112772, a(7)=25456328,
 *            a(8)=306717703
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;        /* cache of already computed terms */
static size_t memo_len = 0;       /* number of terms stored in memo (len>0) */

/* Ensure that memo contains terms up to n, extending and computing as needed. */
static void ensure_upto(idx_t n) {
    if (memo_len == 0) {
        /* allocate space for the first 9 terms (0…8) */
        size_t init_len = 9;
        memo = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!memo) abort();
        memo_len = init_len;
        /* base terms, all fit in 32 bits */
        memo[0] = 1;
        memo[1] = 8;
        memo[2] = 100;
        memo[3] = 1208;
        memo[4] = 14554;
        memo[5] = 175352;
        memo[6] = 2112772;
        memo[7] = 25456328;
        memo[8] = 306717703;
    }
    if ((size_t)n + 1 <= memo_len) return;       /* already computed */

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
    if (!tmp) abort();
    memo = tmp;

    idx_t i;
    for (i = memo_len; i <= n; ++i) {
        /* recurrence uses the last 8 computed values */
        seq_t val = 16 * memo[i-1] + 112 * memo[i-3]
                  + 112 * memo[i-5] + 16 * memo[i-7];
        val -= 56 * memo[i-2];
        val -= 140 * memo[i-4];
        val -= 56 * memo[i-6];
        val -= 2  * memo[i-8];
        memo[i] = val;
    }
    memo_len = new_len;
}

seq_t a(idx_t n) {
    ensure_upto(n);
    return memo[(size_t)n];
}
