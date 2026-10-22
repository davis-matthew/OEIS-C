/*
 * A392025  Least m such that n can be represented as a linear combination of
 *          1^3, 2^3, ..., m^3 with coefficients +-1 only.
 * https://oeis.org/A392025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9088, u32 n <= 9088, u64 n <= 9088,
 *            u128 n <= 9088, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A392025 : Least m such that n can be represented as a linear combination
   of 1^3, 2^3, ..., m^3 with coefficients ±1 only. 
   Offset: 0
   Implementation uses incremental subset-sum DP.
   Complexity:   O(m * S_m) where S_m = (m(m+1)/2)^2, which is < 2½·10⁴ for m≤15.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* m starts from 1, because m=0 yields no terms to assign ±1 coefficients */
    idx_t m = 1;
    seq_t sum = 0;                     /* cumulative sum of cubes up to m */
    size_t dp_len = 1;                 /* dp[0..sum]   */
    bool *dp = (bool *)calloc(dp_len, sizeof(bool));
    dp[0] = true;                      /* empty subset sum = 0 */

    while (true) {
        /* cube of current m */
        seq_t cube = (seq_t)m * m * m;
        sum += cube;                   /* new total sum S_m */

        /* Allocate new DP array for sums up to new sum */
        size_t new_len = (size_t)(sum + 1);
        bool *newdp = (bool *)calloc(new_len, sizeof(bool));

        /* Update DP: for each existing sum i, i and i+cube are reachable */
        for (size_t i = 0; i < dp_len; ++i) {
            if (!dp[i]) continue;
            newdp[i] = true;                          /* keep old sum */
            if (i + (size_t)cube < new_len)           /* add new cube */
                newdp[i + (size_t)cube] = true;
        }
        free(dp);
        dp = newdp;
        dp_len = new_len;

        /* Check if n is reachable with current m */
        if ( ((n + sum) & 1) == 0 ) {            /* parity condition */
            seq_t target = (n + sum) >> 1;        /* (n + S_m) / 2   */
            if (target <= sum) {
                if (dp[(size_t)target]) {
                    free(dp);
                    return (seq_t)m;              /* minimal m found */
                }
            }
        }

        ++m;
    }
}
