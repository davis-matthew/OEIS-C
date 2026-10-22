/*
 * A357716  Number of ways to write n as an ordered sum of eight positive
 *          Fibonacci numbers (with a single type of 1).
 * https://oeis.org/A357716
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 8‑tuples of positive Fibonacci numbers
 *            summing to n
 * Range:     u8 n <= 11, u16 n <= 37, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 8) return 0; /* undefined for n < offset */

    /* Compute Fibonacci numbers (with a single type of 1) up to n */
    idx_t fib[60];           /* plenty of space for any reasonable n */
    size_t fib_cnt = 0;
    idx_t f1 = 1, f2 = 1;

    /* Add the single 1 */
    fib[fib_cnt++] = 1;
    /* Generate subsequent Fibonacci numbers: 2, 3, 5, 8, ... */
    while (1) {
        idx_t f_next = f1 + f2;
        f1 = f2;
        f2 = f_next;
        if (f_next > n) break;
        fib[fib_cnt++] = f_next;
    }

    /* Allocate two DP arrays of size n+1 */
    seq_t *dp_prev = (seq_t*)malloc(sizeof(seq_t) * (n + 1));
    seq_t *dp_curr = (seq_t*)malloc(sizeof(seq_t) * (n + 1));
    if (!dp_prev || !dp_curr) {
        free(dp_prev);
        free(dp_curr);
        return 0; /* allocation failed */
    }

    /* Initialize dp_prev for 0 components */
    memset(dp_prev, 0, sizeof(seq_t) * (n + 1));
    dp_prev[0] = 1;

    /* Perform 8 convolutions */
    for (int t = 0; t < 8; ++t) {
        memset(dp_curr, 0, sizeof(seq_t) * (n + 1));

        for (idx_t s = 1; s <= n; ++s) {
            seq_t sum = 0;
            for (size_t i = 0; i < fib_cnt; ++i) {
                idx_t f = fib[i];
                if (f > s) break;
                sum += dp_prev[s - f];
            }
            dp_curr[s] = sum;
        }

        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    seq_t result = dp_prev[n];
    free(dp_prev);
    free(dp_curr);
    return result;
}
