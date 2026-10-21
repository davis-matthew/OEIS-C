/*
 * A245588  The minimal number of coins needed to make change for n units in a
 *          currency system with coins of value
 *          1,2,5,13,34,89,...,Fibonacci(2k-1).
 * https://oeis.org/A245588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the minimal number of coins of values 1,2,5,13,34,89,…
 *            (Fibonacci numbers at odd indices) needed to make change for n
 *            units. The greedy algorithm works, because each coin is more
 *            than twice the previous coin
 * Range:     u8 n <= 966, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Build the list of coin values up to n.  The number
       of such coins for n ≤ 10 000 is less than 20, so
       a small fixed array suffices. */
    seq_t coins[64];
    size_t coin_cnt = 0;

    seq_t f_prev = 0, f_curr = 1;
    idx_t k = 1;

    while (f_curr <= (seq_t)n) {
        if (k & 1) {                 /* odd index */
            coins[coin_cnt++] = f_curr;
        }
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
        ++k;
    }

    /* Greedy: repeatedly take the largest coin that does
       not exceed the remaining amount. */
    seq_t remaining = (seq_t)n;
    seq_t result = 0;

    for (size_t i = coin_cnt; i > 0; --i) {
        seq_t coin = coins[i - 1];
        if (remaining < coin)
            continue;

        seq_t q = remaining / coin;
        result += q;
        remaining -= q * coin;

        if (remaining == 0)
            break;
    }

    return result;
}
