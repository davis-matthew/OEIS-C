/*
 * A371905  Let k = 0 and let A007814(2*n) = m; If m > k then a(n) = k = k +
 *          m, otherwise a(n) = k = k - m.
 * https://oeis.org/A371905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Let k_0 = 0. * For i = 1 .. n compute m_i = 1 + v_2(i) (i.e. 1
 *            plus the exponent of 2 in i). * Then k_i = k_{i-1} + m_i if m_i
 *            > k_{i-1}; * else k_i = k_{i-1} - m_i. * Finally a(n) = k_n.
 *            (Offset 1: a(1) is the first term listed.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t k = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t t = i;
        idx_t m = 1;
        /* count trailing zeros of i, i.e. v_2(i) */
        while ((t & 1) == 0) {
            t >>= 1;
            ++m;
        }
        seq_t m_seq = (seq_t)m;
        if (m_seq > k) {
            k = k + m_seq;
        } else {
            k = k - m_seq;
        }
    }
    return k;
}
