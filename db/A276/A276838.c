/*
 * A276838  Number of permutations of [n] such that for each cycle c the
 *          smallest integer interval containing all elements of c has at most
 *          four elements.
 * https://oeis.org/A276838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+2*a(n-2)+2*a(n-3)+12*a(n-4)+8*a(n-5)
                    - (2*a(n-6)+5*a(n-7)+a(n-8))
   Initial terms: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=24, a(5)=60, a(6)=150, a(7)=399 */
seq_t a(idx_t n) {
    static const seq_t init[8] = {1,1,2,6,24,60,150,399};
    if (n < 8) return init[n];

    seq_t buf[8];
    for (int i = 0; i < 8; ++i) buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t a0 = buf[0], a1 = buf[1], a2 = buf[2],
              a3 = buf[3], a4 = buf[4], a5 = buf[5],
              a6 = buf[6], a7 = buf[7];

        seq_t sum_pos = a7 + 2 * a6 + 2 * a5 + 12 * a4 + 8 * a3;
        seq_t sum_neg = 2 * a2 + 5 * a1 + a0;
        seq_t next = sum_pos - sum_neg;

        /* shift buffer: discard a0, shift left, insert next */
        for (int k = 0; k < 7; ++k) buf[k] = buf[k + 1];
        buf[7] = next;
    }
    return buf[7];
}
