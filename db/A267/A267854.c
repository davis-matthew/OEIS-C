/*
 * A267854  Binary representation of the n-th iteration of the "Rule 230"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10000*a(n-2)+10000*a(n-4)-100000000*a(n-6) for n>5 * with
 *            a(0)=1, a(1)=110, a(2)=10100, a(3)=1111000, a(4)=101110000, *
 *            a(5)=11101100000
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6) {
        static const seq_t base[6] = { (seq_t)1, (seq_t)110, (seq_t)10100,
                                       (seq_t)1111000, (seq_t)101110000,
                                       (seq_t)11101100000 };
        return base[n];
    }
    seq_t v[6];
    v[0] = (seq_t)1;
    v[1] = (seq_t)110;
    v[2] = (seq_t)10100;
    v[3] = (seq_t)1111000;
    v[4] = (seq_t)101110000;
    v[5] = (seq_t)11101100000;
    const seq_t C10000 = (seq_t)10000;
    const seq_t C100000000 = (seq_t)100000000;
    for (idx_t i = 6; i <= n; ++i) {
        seq_t new_val = C10000 * v[2] + C10000 * v[4] - C100000000 * v[0];
        for (int k = 0; k < 5; ++k) v[k] = v[k + 1];
        v[5] = new_val;
    }
    return v[5];
}
