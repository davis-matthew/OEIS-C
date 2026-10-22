/*
 * A352502  a(n) is the number of integers k in the interval 0..n such that k
 *          and n-k can be added without carries in balanced ternary.
 * https://oeis.org/A352502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 638, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate balanced ternary digit assignments for k such that
 * k and n-k can be added without carries.  Count those with 0 ≤ k ≤ n. */
/* seq_t and idx_t are defined by the build.  No external libraries are required. */

#include <stddef.h>
#include <stdint.h>

/* Helper: recursively explore all valid digit assignments. */
static void dfs(size_t pos, int64_t k, const int *digits, size_t len,
                int64_t n, const int64_t *pow3, seq_t *cnt)
{
    if (pos == len) {
        if (k >= 0 && k <= n)
            (*cnt) += 1;
        return;
    }

    int d = digits[pos];
    int a_start, a_end;
    if (d == 0) {
        a_start = -1; a_end = 1;
    } else if (d == 1) {
        a_start = 0;  a_end = 1;
    } else {          /* d == -1 */
        a_start = -1; a_end = 0;
    }

    for (int a = a_start; a <= a_end; ++a) {
        int64_t newk = k + (int64_t)a * pow3[pos];
        dfs(pos + 1, newk, digits, len, n, pow3, cnt);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Balanced ternary digits of n (least‑significant first). */
    int digits[20];            /* 3^20 >> 64‑bit range, enough for n up to ~3^20 */
    size_t len = 0;
    idx_t temp = n;
    while (temp > 0) {
        idx_t r = temp % 3;
        if (r == 2) {
            digits[len++] = -1;
            temp = (temp + 1) / 3;
        } else {
            digits[len++] = (int)r;   /* r == 0 or 1 */
            temp /= 3;
        }
    }
    /* For n == 0, len == 0; dfst will count the single valid k == 0. */

    /* Pre‑compute powers of 3 up to len-1. */
    int64_t pow3[20];
    pow3[0] = 1;
    for (size_t i = 1; i < len; ++i)
        pow3[i] = pow3[i - 1] * 3LL;

    seq_t cnt = 0;
    dfs(0, 0, digits, len, (int64_t)n, pow3, &cnt);
    return cnt;
}
