/*
 * A363800  Expansion of Product_{k>0} (1 - x^(7*k-5)) * (1 - x^(7*k-2)) * (1
 *          - x^(7*k)).
 * https://oeis.org/A363800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 98, i16 n <= 98, i32 n <= 98, i64 n <= 98, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{k∈ℤ} (-1)^k * [k(7k+3)/2 = n]            (Kronecker delta)
 * The function evaluates this sum directly for any 0 ≤ n.
 * It uses the closed form of the exponents and checks the sign via the parity of k.
 */

seq_t a(idx_t n)
{
    /* Exponent 0 occurs for k = 0, giving a(0) = +1 */
    if (n == 0)
        return 1;

    seq_t value = 0;
    seq_t sn = (seq_t)n;

    /* Handle k > 0 : exponent k(7k+3)/2 */
    for (idx_t k = 1; ; ++k) {
        seq_t m = (seq_t)k * (((seq_t)7 * k) + 3) / 2;
        if (m > sn)
            break;
        if (m == sn)
            value = ((k & 1) ? -1 : 1);   /* (-1)^k */
    }

    /* Handle k < 0 : exponent |k|(7|k|-3)/2, sign same as for |k| */
    for (idx_t k = 1; ; ++k) {
        seq_t m = (seq_t)k * (((seq_t)7 * k) - 3) / 2;
        if (m > sn)
            break;
        if (m == sn)
            value = ((k & 1) ? -1 : 1);   /* (-1)^k with k>0 = (-1)^(-k) */
    }

    return value;
}
