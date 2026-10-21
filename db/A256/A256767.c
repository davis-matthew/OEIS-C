/*
 * A256767  Number of (n+2)X(4+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum less than the antidiagonal sum.
 * https://oeis.org/A256767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(R_even)^2 * f(R_odd)^2 * where the number of rows is
 *            n+2, R_even = floor((n+3)/2), * R_odd = floor((n+2)/2), and *
 *            f(r) is the number of binary 0..1 r × 3 arrays * with each 2×2
 *            subblock satisfying a+d >= b+c
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 23, u128 n <= 55,
 *            big any n
 * Helpers:   compute_transition_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_transition_ok.h"

/* transition table between two consecutive rows (3 bits each) */
static bool transition_ok[8][8];
static bool transition_initialized = false;

static inline void init_transition(void)
{
    if (!transition_initialized) {
        for (int p1 = 0; p1 < 8; ++p1)
            for (int p2 = 0; p2 < 8; ++p2)
                transition_ok[p1][p2] = compute_transition_ok(p1, p2);
        transition_initialized = true;
    }
}

/* Compute f(r): number of r×3 binary arrays with the local constraint. */
static seq_t compute_f(idx_t r)
{
    init_transition();

    if (r == 0) return (seq_t)1;
    seq_t dp[8], next[8];

    /* first row: all patterns are admissible */
    for (int p = 0; p < 8; ++p) dp[p] = (seq_t)1;

    for (idx_t row = 2; row <= r; ++row) {
        for (int cur = 0; cur < 8; ++cur) next[cur] = 0;
        for (int prev = 0; prev < 8; ++prev) {
            seq_t val = dp[prev];
            /* propagate only if value is non‑zero; skip check for simplicity */
            for (int cur = 0; cur < 8; ++cur) {
                if (transition_ok[prev][cur])
                    next[cur] += val;
            }
        }
        for (int cur = 0; cur < 8; ++cur) dp[cur] = next[cur];
    }

    seq_t total = 0;
    for (int p = 0; p < 8; ++p) total += dp[p];
    return total;
}

seq_t a(idx_t n)
{
    /* number of rows in the full array */
    idx_t rows = n + 2;

    idx_t r_even = (rows + 1) / 2;  /* floor((rows+1)/2)  */
    idx_t r_odd  = rows / 2;        /* floor(rows/2)      */

    seq_t fe = compute_f(r_even);
    seq_t fo = compute_f(r_odd);

    seq_t res = fe * fe;   /* fe^2 */
    res = res * fo;        /* fe^2 * fo */
    res = res * fo;        /* fe^2 * fo^2 = a(n) */

    return res;
}
