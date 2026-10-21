/*
 * A252715  Number of (n+2) X (4+2) 0..3 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(1) = 96, a(2) = 63, a(3) = 70, a(4) = 97,
 *            a(5) = 135, a(6) = 178, a(7) = 257, a(8) = 368, a(9) = 492,
 *            a(10) = 722, a(11) = 1047, a(12) = 1406. For n > 12: a(n) =
 *            a(n-1) + 6*a(n-3) - 6*a(n-4) - 10*a(n-6) + 10*a(n-7) + 3*a(n-9)
 *            - 3*a(n-10)
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 53, u64 n <= 114, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[12] = {
    96,   63,   70,   97,  135,  178,
   257,  368,  492,  722, 1047, 1406
};

seq_t a(idx_t n)
{
    /* Memoization cache: memo[i] holds a(i) for i >= 1 */
    static seq_t *memo = NULL;
    static size_t memo_size = 0;

    if (n == 0) return 0;             /* not used by caller, but safe */
    if (n <= 12) return base_vals[n-1];

    /* Ensure cache big enough to hold up to n */
    if (memo_size < n + 1) {
        size_t new_size = (n + 1 > 13) ? (n + 1) : 13;
        seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        memo = tmp;
        /* initialize new entries to 0 */
        for (size_t i = memo_size; i < new_size; ++i) memo[i] = 0;
        /* fill known base values if not already set */
        for (size_t i = 1; i <= 12 && i < new_size; ++i)
            memo[i] = base_vals[i-1];
        memo_size = new_size;
    }

    /* Compute values from current cache size up to n */
    for (idx_t i = (idx_t)13; i <= n; ++i) {
        seq_t val = memo[i-1];
        val += 6 * memo[i-3];
        val -= 6 * memo[i-4];
        val -= 10 * memo[i-6];
        val += 10 * memo[i-7];
        val += 3 * memo[i-9];
        val -= 3 * memo[i-10];
        memo[i] = val;
    }

    return memo[n];
}
