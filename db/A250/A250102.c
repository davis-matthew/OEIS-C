/*
 * A250102  a(n) = 2*5^n - (1+2i)^(2n) - (1-2i)^(2n) where i = sqrt(-1).
 * https://oeis.org/A250102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -a(n-1) + 5*a(n-2) + 125*a(n-3); a(0)=0, a(1)=16, a(2)=64
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0;
    if (n == 1U) return (seq_t)16;
    if (n == 2U) return (seq_t)64;

    seq_t a_nm3 = (seq_t)0;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)16;  /* a(n-2)  */
    seq_t a_nm1 = (seq_t)64;  /* a(n-1)  */

    for (idx_t i = 3U; i <= n; ++i) {
        /* a(n) = -a(n-1) + 5*a(n-2) + 125*a(n-3)  */
        seq_t sum   = (seq_t)5 * a_nm2 + (seq_t)125 * a_nm3;
        seq_t a_n   = sum - a_nm1;    /* sum ≥ a_nm1 for n ≥ 3 */

        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
