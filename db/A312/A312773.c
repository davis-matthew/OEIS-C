/*
 * A312773  Coordination sequence Gal.5.276.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2*a(n-1) -2*a(n-2) -2*a(n-3) -2*a(n-4) * +3*a(n-6)
 *            +4*a(n-7)+4*a(n-8)+4*a(n-9)+3*a(n-10) *
 *            -2*a(n-12)-2*a(n-13)-2*a(n-14)-2*a(n-15)-a(n-16), * with a(0)=1,
 *            a(1)=4, a(2)=8, a(3)=16, a(4)=18, a(5)=24, a(6)=26, * a(7)=36,
 *            a(8)=38, a(9)=44, a(10)=44, a(11)=52, a(12)=56, * a(13)=70,
 *            a(14)=58,...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Initial 18 terms in the sequence. */
    static const seq_t init[18] = {
        (seq_t)1,  (seq_t)4,  (seq_t)8,  (seq_t)16,
        (seq_t)18, (seq_t)24, (seq_t)26, (seq_t)36,
        (seq_t)38, (seq_t)44, (seq_t)44, (seq_t)52,
        (seq_t)56, (seq_t)70, (seq_t)58, (seq_t)78,
        (seq_t)64, (seq_t)90
    };

    if (n < 18) {
        return init[n];
    }

    /* Circular buffer holding the last 18 computed values.
     * buf[0] = a(i-18), buf[17] = a(i-1) when processing i. */
    seq_t buf[18];
    for (int i = 0; i < 18; ++i) {
        buf[i] = init[i];
    }

    for (idx_t i = 18; i <= n; ++i) {
        /* Compute a(i) using the recurrence.  Use a positive
         * sum minus a negative sum to avoid temporary negatives. */
        seq_t pos = 3 * buf[12]          /* a(i-6) */
                 + 4 * buf[11]          /* a(i-7) */
                 + 4 * buf[10]          /* a(i-8) */
                 + 4 * buf[ 9]          /* a(i-9) */
                 + 3 * buf[ 8];         /* a(i-10) */

        seq_t neg = 2 * buf[17]          /* a(i-1) */
                 + 2 * buf[16]          /* a(i-2) */
                 + 2 * buf[15]          /* a(i-3) */
                 + 2 * buf[14]          /* a(i-4) */
                 + 2 * buf[ 6]          /* a(i-12) */
                 + 2 * buf[ 5]          /* a(i-13) */
                 + 2 * buf[ 4]          /* a(i-14) */
                 + 2 * buf[ 3]          /* a(i-15) */
                 +     buf[ 2];         /* a(i-16) */

        seq_t val = pos - neg;

        /* Shift buffer left: idx 0 becomes 1, ..., 16 becomes 17. */
        for (int j = 0; j < 17; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[17] = val;
    }

    return buf[17];
}
