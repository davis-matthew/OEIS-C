/*
 * A372242  a(n) = 10*Fibonacci(n) + (-1)^n.
 * https://oeis.org/A372242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*Fibonacci(n) + (-1)^n
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) iteratively; Fibonacci(0)=0 */
    seq_t f_prev = (seq_t)0;
    seq_t f_curr = (seq_t)1;

    if (n != 0) {
        for (idx_t i = 1; i <= n; ++i) {
            seq_t f_next = f_prev + f_curr;
            f_prev = f_curr;
            f_curr = f_next;
        }
    }

    seq_t fn = f_prev;  /* Fibonacci(n) */

    seq_t res = fn * (seq_t)10;

    /* Add or subtract 1 depending on parity of n */
    if (n & (idx_t)1)
        res = res - (seq_t)1;
    else
        res = res + (seq_t)1;

    return res;
}
