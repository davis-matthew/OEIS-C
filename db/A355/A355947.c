/*
 * A355947  a(n) = Sum_{k=1..n} (n+1-k)*floor(n/k).
 * https://oeis.org/A355947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (n+1-k) * floor(n/k)
 * Range:     u8 n <= 11, u16 n <= 124, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sn = (seq_t)n;        /* n as seq_t */
    seq_t result = 0;
    idx_t k = 1;

    while (k <= n) {
        idx_t vi = n / k;               /* floor(n/k) */
        seq_t v = (seq_t)vi;            /* cast to seq_t for multiplication */
        idx_t k_next = n / vi + 1;      /* first k where floor changes */
        idx_t e = k_next - 1;           /* last k in this block */
        idx_t cnt = e - k + 1;          /* number of terms in block */

        /* sum_{t=k}^{e} (n+1-t) = cnt*(n+1) - (k+e)*cnt/2 */
        seq_t term_sum = (seq_t)cnt * (sn + 1)
                        - ((seq_t)k + (seq_t)e) * (seq_t)cnt / 2;

        result += term_sum * v;
        k = k_next;
    }

    return result;
}
