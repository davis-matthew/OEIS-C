/*
 * A395203  a(n) = (2*n)! * [x^(2*n)] (f(x) + f(-x))/2 and f(x) = 1/sqrt(1 +
 *          2*sqrt(2)*x + x^2).
 * https://oeis.org/A395203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! * P_{2n}(√2) where P_k(x) is the Legendre
 *            polynomial. Recurrence for A_m = m!·P_m(√2) = u_m + v_m·√2: A_0
 *            = 1 → u_0=1, v_0=0 A_1 = √2 → u_1=0, v_1=1 For m ≥ 1: A_{m+1} =
 *            (2m+1)·√2·A_m – m²·A_{m-1} This yields integer values for even
 *            indices, so a(n) = A_{2n} = u_{2n}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = 2*n, the index of the Legendre coefficient we need */
    idx_t m = n * 2;
    if (m == 0)
        return (seq_t)1;   /* a(0) = 1 */

    /* initial values for A_0 and A_1 */
    seq_t u_prevprev = (seq_t)1, v_prevprev = (seq_t)0; /* A_0 */
    seq_t u_prev     = (seq_t)0, v_prev     = (seq_t)1; /* A_1 */

    /* iterate the recurrence for A_{m}  (m from 1 up to m_target-1) */
    for (idx_t i = 1; i < m; ++i)
    {
        /* i corresponds to the current m in the recurrence */
        seq_t i_seq = (seq_t)i;
        seq_t mul   = i_seq * i_seq;                          /* i² */

        /* compute u_cur = (2i+1)*(2*v_prev) – i²*u_prevprev */
        seq_t term1 = ((seq_t)(2 * i + 1)) * ((seq_t)2 * v_prev);
        seq_t term2 = mul * u_prevprev;
        seq_t u_cur = term1 - term2;

        /* compute v_cur = (2i+1)*u_prev – i²*v_prevprev */
        seq_t term3 = ((seq_t)(2 * i + 1)) * u_prev;
        seq_t term4 = mul * v_prevprev;
        seq_t v_cur = term3 - term4;

        /* shift for next iteration */
        u_prevprev = u_prev;   v_prevprev = v_prev;
        u_prev     = u_cur;    v_prev     = v_cur;
    }

    /* for even m the v component is zero; u_cur holds the result */
    return u_prev;
}
