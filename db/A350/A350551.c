/*
 * A350551  Convolution of Jacobsthal numbers and Pell numbers.
 * https://oeis.org/A350551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (P(n+1) + P(n) - J(n+2))/2 where P is Pell and J is
 *            Jacobsthal
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pell numbers: P(0)=0, P(1)=1, P(k)=2*P(k-1)+P(k-2) */
    seq_t P_nm2 = (seq_t)0;   /* P(k-2) */
    seq_t P_nm1 = (seq_t)1;   /* P(k-1) */
    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t P_n = 2 * P_nm1 + P_nm2;
        P_nm2 = P_nm1;
        P_nm1 = P_n;
    }
    seq_t P_n   = P_nm2; /* P(n)   */
    seq_t P_np1 = P_nm1; /* P(n+1) */

    /* Jacobsthal numbers: J(0)=0, J(1)=1, J(k)=J(k-1)+2*J(k-2) */
    seq_t J_nm2 = (seq_t)0;   /* J(k-2) */
    seq_t J_nm1 = (seq_t)1;   /* J(k-1) */
    for (idx_t i = 2; i <= n + 2; ++i) {
        seq_t J_n = J_nm1 + 2 * J_nm2;
        J_nm2 = J_nm1;
        J_nm1 = J_n;
    }
    seq_t J_np2 = J_nm1; /* J(n+2) */

    seq_t result = (P_np1 + P_n - J_np2) / (seq_t)2;
    return result;
}
