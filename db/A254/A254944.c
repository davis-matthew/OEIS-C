/*
 * A254944  Number of length n 1..(5+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5, 7 or 11.
 * https://oeis.org/A254944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n arrays with elements 1..6 such that
 *            each leading partial sum is divisible by at least one of
 *            2,3,5,7,11
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define L       2310
#define MAX_N   20000

static bool        valid_res[L];
static seq_t       dp0[L];
static seq_t       dp1[L];
static seq_t      *dp_cur  = dp0;
static seq_t      *dp_next = dp1;
static seq_t       memo[MAX_N+1];
static idx_t       memo_last  = 0;
static seq_t       last_total = 0;
static bool        initialized = false;

seq_t a(idx_t n) {
    if (!initialized) {
        for (int i = 0; i < L; ++i) {
            valid_res[i] = ((i % 2 == 0) || (i % 3 == 0) || (i % 5 == 0) ||
                            (i % 7 == 0) || (i % 11 == 0));
            dp0[i] = 0;
            dp1[i] = 0;
        }
        dp_cur[0] = 1;          /* empty prefix */
        initialized = true;
    }

    if (n <= memo_last)
        return memo[n];

    for (idx_t step = memo_last + 1; step <= n; ++step) {
        /* clear dp_next */
        for (int i = 0; i < L; ++i) dp_next[i] = 0;

        /* transition from dp_cur to dp_next */
        for (int res = 0; res < L; ++res) {
            seq_t val = dp_cur[res];
            if (val == 0)
                continue;
            for (int a_val = 1; a_val <= 6; ++a_val) {
                int new_res = res + a_val;
                if (new_res >= L)
                    new_res -= L;
                if (valid_res[new_res]) {
                    dp_next[new_res] = dp_next[new_res] + val;
                }
            }
        }

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur    = dp_next;
        dp_next   = tmp;

        /* sum all counts to get a(step) */
        seq_t total = 0;
        for (int i = 0; i < L; ++i)
            total = total + dp_cur[i];

        if (step <= MAX_N)
            memo[step] = total;
        last_total = total;
        memo_last  = step;
    }

    if (n <= MAX_N)
        return memo[n];
    return last_total;
}
