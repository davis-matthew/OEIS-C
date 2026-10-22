/*
 * A355218  a(n) = Sum_{k>=1} (3*k - 1)^n / 2^k.
 * https://oeis.org/A355218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + Sum_{k=1..n} binomial(n,k) * 3^k * a(n-k) a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static idx_t memo_cap = 0;
    static idx_t memo_last = 0;

    static seq_t *pow2_arr = NULL;
    static seq_t *pow3_arr = NULL;
    static idx_t pow_cap = 0;

    /* ensure memo array large enough */
    if (n >= memo_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!new_memo) return 0;
        if (memo_cap == 0) {
            new_memo[0] = 1;          /* a(0) = 1 */
            memo_last = 0;
            for (idx_t i = 1; i < new_cap; ++i)
                new_memo[i] = 0;
        } else {
            for (idx_t i = memo_cap; i < new_cap; ++i)
                new_memo[i] = 0;
        }
        memo = new_memo;
        memo_cap = new_cap;
    }

    /* ensure power arrays large enough */
    if (n >= pow_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_pow2 = (seq_t *)realloc(pow2_arr, new_cap * sizeof(seq_t));
        seq_t *new_pow3 = (seq_t *)realloc(pow3_arr, new_cap * sizeof(seq_t));
        if (!new_pow2 || !new_pow3) return 0;
        if (pow_cap == 0) {
            new_pow2[0] = 1;
            new_pow3[0] = 1;
        }
        for (idx_t i = (pow_cap == 0) ? 1 : pow_cap; i < new_cap; ++i) {
            new_pow2[i] = new_pow2[i - 1] * (seq_t)2;
            new_pow3[i] = new_pow3[i - 1] * (seq_t)3;
        }
        pow2_arr = new_pow2;
        pow3_arr = new_pow3;
        pow_cap = new_cap;
    }

    if (n <= memo_last) return memo[n];

    /* compute missing terms via recurrence */
    for (idx_t i = memo_last + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;
        for (idx_t k = 1; k <= i; ++k) {
            binom = binom * (seq_t)(i - k + 1) / k;
            seq_t term = binom * pow3_arr[k] * memo[i - k];
            sum += term;
        }
        memo[i] = pow2_arr[i] + sum;
    }
    memo_last = n;
    return memo[n];
}
