/*
 * A247729  Number of length 3+3 0..n arrays with no disjoint pairs in any
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n-1)+a(n-2)-2*a(n-3)-2*a(n-4)-2*a(n-5)+5*a(n-6)+2*a(n-7)-2*a(n-9)-5*a(n-10)+2*a(n-11)+2*a(n-12)+2*a(n-13)-a(n-14)-2*a(n-15)+a(n-16);
 *            Initial values: a(1)=8, a(2)=172, a(3)=1248, a(4)=5796,
 *            a(5)=19744, a(6)=55372, a(7)=133780, a(8)=290004, a(9)=576064,
 *            a(10)=1068584, a(11)=1871996, a(12...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* store the 16 known starting values */
    static seq_t base[17];
    static int initialized = 0;
    if (!initialized) {
        base[1] = 8;
        base[2] = 172;
        base[3] = 1248;
        base[4] = 5796;
        base[5] = 19744;
        base[6] = 55372;
        base[7] = 133780;
        base[8] = 290004;
        base[9] = 576064;
        base[10] = 1068584;
        base[11] = 1871996;
        base[12] = 3129068;
        base[13] = 5023940;
        base[14] = 7795872;
        base[15] = 11741676;
        base[16] = 17233232;
        initialized = 1;
    }

    if (n <= 16) {
        return base[n];
    }

    /* last[0] = a(k-1), last[1] = a(k-2), ..., last[15] = a(k-16)   */
    seq_t last[16];
    for (int i = 0; i < 16; ++i) {
        last[i] = base[16 - i];
    }

    for (idx_t k = 17; k <= n; ++k) {
        seq_t next =
            2 * last[0] + last[1]
            - 2 * last[2] - 2 * last[3] - 2 * last[4]
            + 5 * last[5] + 2 * last[6]
            - 2 * last[8] - 5 * last[9]
            + 2 * last[10] + 2 * last[11] + 2 * last[12]
            - last[13] - 2 * last[14] + last[15];

        /* shift the window: drop a(k-16), shift others, insert new value */
        for (int i = 15; i > 0; --i) {
            last[i] = last[i - 1];
        }
        last[0] = next;
    }
    return last[0];
}
