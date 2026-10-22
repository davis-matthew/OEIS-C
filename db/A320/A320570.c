/*
 * A320570  a(n) = L_n(n), where L_n(x) is the Lucas polynomial.
 * https://oeis.org/A320570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n + sqrt(n^2 + 4))^n + (n - sqrt(n^2 + 4))^n)/2^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via the Lucas polynomial recurrence:  L0 = 2, L1 = n, */
/* Lk = n * Lk-1 + Lk-2  (for k ≥ 2) */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)2;
    }
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t Lprevprev = (seq_t)2;        /* L0 */
    seq_t Lprev     = (seq_t)n;        /* L1 */
    seq_t Lcurr;

    for (idx_t k = 2; k <= n; ++k) {
        Lcurr = (seq_t)n * Lprev + Lprevprev;
        Lprevprev = Lprev;
        Lprev = Lcurr;
    }

    return Lprev;                      /* L_n(n) */
}
