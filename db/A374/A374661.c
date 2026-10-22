/*
 * A374661  a(n) = (1/6)*A374660(n).
 * https://oeis.org/A374661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=1}^n (L_k + 4), where L_k is the k‑th Lucas
 *            number: L_0 = 2, L_1 = 1, L_k = L_{k-1} + L_{k-2} (k ≥ 2). a(0)
 *            = 1, a(1) = 5, a(2) = 35, a(3) = 280, a(4) = 3080, …
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t res = 1;
    /* Lucas numbers: L_0 = 2, L_1 = 1 */
    seq_t luc_prev      = 1;   /* L_k, starting with k = 1          */
    seq_t luc_prevprev  = 2;   /* L_{k-1} for the next iteration   */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t factor = luc_prev + (seq_t)4;
        res *= factor;
        /* compute next Lucas number: L_{k+1} = L_k + L_{k-1} */
        seq_t next = luc_prev + luc_prevprev;
        luc_prevprev = luc_prev;
        luc_prev = next;
    }

    return res;
}
