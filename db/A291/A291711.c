/*
 * A291711  The minimum number of coins needed to pay for n units in the
 *          currency system of values 1, 3, 8, 21, 55, 144, ...,
 *          Fibonacci(2k), ...
 * https://oeis.org/A291711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1367, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A291711: minimum number of coins with values Fibonacci(2k) that sum to n.
   A greedy algorithm is used: repeatedly take the largest available coin. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined on the offset 1; 0 is not used */

    const seq_t target = (seq_t)n;
    seq_t coins[64];
    size_t coin_cnt = 0;

    /* generate Fibonacci numbers and pick even‑indexed ones ≤ target */
    seq_t f_prev2 = 0;          /* F(0) */
    seq_t f_prev1 = 1;          /* F(1) */
    seq_t f_curr = 0;
    size_t idx = 2;             /* current Fibonacci index */

    while (1) {
        f_curr = f_prev2 + f_prev1;      /* F(idx) */

        if ((idx & 1) == 0) {            /* even index → coin candidate */
            if (f_curr <= target) {
                coins[coin_cnt++] = f_curr;
            }
        }

        if (f_curr > target) break;      /* next even coin would also be > target */

        /* advance Fibonacci */
        f_prev2 = f_prev1;
        f_prev1 = f_curr;
        ++idx;
    }

    /* greedy coin counting */
    seq_t rem = target;
    seq_t count = 0;
    for (size_t i = coin_cnt; i > 0; ) {
        --i;
        seq_t coin = coins[i];
        if (rem >= coin) {
            seq_t q = rem / coin;          /* how many of this coin */
            count += q;
            rem -= q * coin;
        }
    }

    return count;
}
