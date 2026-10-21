/*
 * A243725  Number of length n+2 0..6 arrays with no three unequal elements in
 *          a row and no three equal elements in a row and new values 0..6
 *          introduced in 0..6 order.
 * https://oeis.org/A243725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 24, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A243725: Number of length n+2 0..6 arrays with no three unequal
   elements in a row and no three equal elements in a row and new values
   0..6 introduced in 0..6 order.
   Empirical linear recurrence (valid for n ≥ 10):
     a(n) = 6*a(n-1) - 54*a(n-3) + 50*a(n-4) + 172*a(n-5)
            - 181*a(n-6) - 244*a(n-7) + 180*a(n-8) + 144*a(n-9)
   Initial values (n = 1..9):
     a(1)=3, a(2)=6, a(3)=12, a(4)=25, a(5)=53, a(6)=116,
     a(7)=259, a(8)=594, a(9)=1389.
   The function computes the value directly using the recurrence, in
   time O(n) and with no dynamic allocation, so it works for any order
   of calls.
*/

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    /* Initial terms 1..9 */
    static const seq_t init[9] = {
        (seq_t)3, (seq_t)6, (seq_t)12, (seq_t)25,
        (seq_t)53, (seq_t)116, (seq_t)259, (seq_t)594,
        (seq_t)1389
    };

    if (n <= 9U) {
        return init[(size_t)(n - 1)];
    }

    /* Buffer to keep the last 9 values: buf[0] = a[i-9], ..., buf[8] = a[i-1] */
    seq_t buf[9];
    for (int k = 0; k < 9; ++k) {
        buf[k] = init[k];
    }

    for (idx_t i = 10; i <= n; ++i) {
        /* Compute a[i] using the recurrence and the buffer */
        seq_t pos = (seq_t)6   * buf[8];   /* 6*a[i-1]  */
        pos += (seq_t)50  * buf[5];        /* 50*a[i-4] */
        pos += (seq_t)172 * buf[4];        /* 172*a[i-5] */
        pos += (seq_t)180 * buf[1];        /* 180*a[i-8] */
        pos += (seq_t)144 * buf[0];        /* 144*a[i-9] */

        seq_t neg = (seq_t)54  * buf[6];    /* 54*a[i-3]  */
        neg += (seq_t)181 * buf[3];        /* 181*a[i-6] */
        neg += (seq_t)244 * buf[2];        /* 244*a[i-7] */

        seq_t next = pos - neg;            /* final value a[i] */

        /* Shift buffer left by one position and insert the new value */
        for (int k = 0; k < 8; ++k) {
            buf[k] = buf[k + 1];
        }
        buf[8] = next;
    }

    return buf[8];
}
