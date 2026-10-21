/*
 * A243727  Number of length n+2 0..8 arrays with no three unequal elements in
 *          a row and no three equal elements in a row and new values 0..8
 *          introduced in 0..8 order.
 * https://oeis.org/A243727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 24, u64 n <= 43, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Dynamic programming over the last two digits.
   State (k,a,b) :  k = number of distinct digits already used (1≤k≤9),
   a = element at position (current_length‑2), b = element at position
   (current_length‑1).  From a valid state we may add a new element s
   (0≤s≤8) if the triple (a,b,s) contains exactly two equal values.
   Additionally s must be ≤k and if s==k a new digit is introduced.
   The initial length‑2 arrays are 0,0 and 0,1 (since the first digit
   must be 0 and new digits appear in order 0,1,…).  After reaching
   length n+2, the sum of all states gives a(n). */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* local DP tables: cur holds counts for current length,
       next holds counts for the next length. */
    static seq_t cur[10][9][9];
    static seq_t next[10][9][9];
    /* zero the tables */
    for (int k = 1; k <= 9; ++k)
        for (int a = 0; a < 9; ++a)
            for (int b = 0; b < 9; ++b)
                cur[k][a][b] = next[k][a][b] = 0;

    /* starting length = 2 */
    cur[1][0][0] = 1;   /* 0,0  (only digit 0 used) */
    cur[2][0][1] = 1;   /* 0,1  (digits 0 and 1 used) */

    idx_t target_len = n + 2;
    for (idx_t len = 2; len < target_len; ++len) {
        /* zero next */
        for (int k = 1; k <= 9; ++k)
            for (int a = 0; a < 9; ++a)
                for (int b = 0; b < 9; ++b)
                    next[k][a][b] = 0;

        for (int k = 1; k <= 9; ++k) {
            for (int a = 0; a < 9; ++a) {
                for (int b = 0; b < 9; ++b) {
                    seq_t cnt = cur[k][a][b];
                    if (!cnt) continue;

                    /* try existing digits 0..k-1 */
                    for (int s = 0; s < k; ++s) {
                        if ((a == b && b == s) ||          /* all equal   */
                            (a != b && a != s && b != s))  /* all distinct */
                            continue;
                        next[k][b][s] += cnt;
                    }

                    /* try new digit k */
                    if (k < 9) {
                        int s = k;
                        if (!((a == b && b == s) || (a != b && a != s && b != s))) {
                            next[k + 1][b][s] += cnt;
                        }
                    }
                }
            }
        }
        /* advance */
        for (int k = 1; k <= 9; ++k)
            for (int a = 0; a < 9; ++a)
                for (int b = 0; b < 9; ++b)
                    cur[k][a][b] = next[k][a][b];
    }

    /* sum all states at length n+2 */
    seq_t total = 0;
    for (int k = 1; k <= 9; ++k)
        for (int a = 0; a < 9; ++a)
            for (int b = 0; b < 9; ++b)
                total += cur[k][a][b];

    return total;
}
