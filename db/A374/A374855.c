/*
 * A374855  a(n) = (1/6)*Product_{k=0..n} F(k)+6, where F=A000045 (Fibonacci
 *          numbers).
 * https://oeis.org/A374855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} (F_k + 6) where F_k is the k‑th Fibonacci
 *            number
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t f_prev = 0;        /* F_{k-1}  */
    seq_t f_curr = 1;        /* F_k      */
    for (idx_t i = 1; i <= n; ++i) {
        result *= (f_curr + (seq_t)6);   /* multiply by (F_k + 6) */
        /* advance Fibonacci */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
    return result;
}
