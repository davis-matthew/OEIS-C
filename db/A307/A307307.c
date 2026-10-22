/*
 * A307307  a(n) is the forgotten index of the Lucas cube Lambda(n).
 * https://oeis.org/A307307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A307307: Forgotten index of the Lucas cube Lambda(n).
   Recurrence (valid for n > 11):
     a(n) = 4*a(n-1) - 2*a(n-2) - 8*a(n-3) + 5*a(n-4) + 8*a(n-5) - 2*a(n-6) - 4*a(n-7) - a(n-8)
   Initial values (offset = 1):
     a(1) = 0,  a(2) = 10,  a(3) = 30,  a(4) = 112,  a(5) = 300,
     a(6) = 840, a(7) = 2044, a(8) = 4864, a(9) = 10944, a(10) = 23960,
     a(11) = 50908
*/

seq_t a(idx_t n) {
    /* base terms up to n = 11 */
    static const seq_t base[11] = {
        (seq_t)0, (seq_t)10, (seq_t)30, (seq_t)112, (seq_t)300,
        (seq_t)840, (seq_t)2044, (seq_t)4864, (seq_t)10944,
        (seq_t)23960, (seq_t)50908
    };

    if (n <= 11) {
        return base[n - 1];
    }

    /* last[0] = a(k-1), last[1] = a(k-2), ..., last[7] = a(k-8) */
    seq_t last[8] = {
        base[10],   /* a(11) */
        base[9],    /* a(10) */
        base[8],    /* a(9)  */
        base[7],    /* a(8)  */
        base[6],    /* a(7)  */
        base[5],    /* a(6)  */
        base[4],    /* a(5)  */
        base[3]     /* a(4)  */
    };

    seq_t next;
    for (idx_t k = 12; k <= n; ++k) {
        /* compute a(k) from the recurrence */
        next = (seq_t)4 * last[0]
             - (seq_t)2 * last[1]
             - (seq_t)8 * last[2]
             + (seq_t)5 * last[3]
             + (seq_t)8 * last[4]
             - (seq_t)2 * last[5]
             - (seq_t)4 * last[6]
             - last[7];

        /* shift window to make room for next term */
        for (int j = 7; j > 0; --j) {
            last[j] = last[j - 1];
        }
        last[0] = next;
    }

    return last[0];
}
