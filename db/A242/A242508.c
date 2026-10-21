/*
 * A242508  Number of compositions of n, where the difference between the
 *          number of odd parts and the number of even parts is 10.
 * https://oeis.org/A242508
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of compositions of n into positive integers in
 *            which the difference (#odd parts − #even parts) equals 10. The
 *            function implements a direct summation over all feasible numbers
 *            of even parts e and all feasible sums of even parts S_e. For
 *            each pair (e,S_e) it multiplies the number...
 * Range:     u8 n <= 15, u16 n <= 22, u32 n <= 36, u64 n <= 69, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_nk(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * (seq_t)(N - K + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Offset 10 – the first term is defined for n = 10 */
    if (n < 10) return 0;
    if (n == 10) return 1;
    if (n == 11) return 0;
    if (n == 12) return 10;
    if (n == 13) return 12;

    seq_t sum = 0;
    idx_t e_max = (n >= 10) ? ((n - 10) / 3) : 0;

    for (idx_t e = 0; e <= e_max; ++e) {
        /* Parity condition: n and e must have the same parity. */
        if ((n & 1) != (e & 1)) continue;

        idx_t o = e + 10;           /* number of odd parts */
        idx_t t_min = 2 * e;        /* minimal sum of even parts */
        idx_t t_max;

        /* For e == 0 the sum of even parts must be zero. */
        if (e == 0) {
            t_max = t_min;
        } else {
            t_max = n - o;
            if (t_max < t_min) continue;
            if (t_max & 1) --t_max;    /* keep even */
        }

        /* Number of ways to arrange the parity of the parts. */
        seq_t part_arr = binom_nk(2 * e + 10, e + 10);

        /* Initial values for even part distribution. */
        seq_t B_even = 1;
        idx_t m_e = 0;                /* current value of (t/2 - 1) */
        if (e > 0) m_e = e - 1;

        /* Initial values for odd part distribution. */
        idx_t diff = n - e - 10;                 /* = n - o            */
        idx_t m_o_start = diff / 2 + 9;          /* starts as (n‑e‑10)/2 + 9 */
        idx_t k_o = e + 9;                       /* fixed k for this e */
        seq_t B_odd = binom_nk(m_o_start, k_o);
        idx_t m_o = m_o_start;                   /* mutable current m_o */

        for (idx_t t = t_min; t <= t_max; t += 2) {
            /* Contribution for this (e, S_e = t). */
            seq_t contrib = part_arr * B_odd * B_even;
            sum += contrib;

            if (t + 2 > t_max) break;           /* last iteration */

            /* Update B_odd for next t (decrease m_o by 1). */
            seq_t num_odd = (seq_t)(m_o - k_o);
            seq_t den_odd = (seq_t)m_o;
            B_odd = B_odd * num_odd / den_odd;
            --m_o;

            /* Update B_even for next t (increase m_e by 1). */
            if (e > 0) {
                seq_t num_even = (seq_t)(m_e + 1);
                seq_t den_even = (seq_t)(m_e + 2 - e);
                B_even = B_even * num_even / den_even;
                ++m_e;
            }
        }
    }
    return sum;
}
