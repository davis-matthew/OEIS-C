/*
 * A275651  a(n) = (2*n)!*Sum_{k = 0..n} (-1)^k/(2*k)!.
 * https://oeis.org/A275651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)! * Σ_{k=0..n} (-1)^k/(2*k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1,  a(n) = 2*n*(2*n-1)*a(n-1) + (-1)^n */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;            /* a(0) */
    if (n == 0) return result;

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute the multiplier 2*i*(2*i-1) */
        idx_t two = 2 * i;
        seq_t mult = (seq_t)two * ((seq_t)two - 1);

        /* Apply the recurrence */
        result = result * mult;
        if ((i & 1) == 0) {           /* (-1)^i = +1 when i even */
            result += (seq_t)1;
        } else {                      /* (-1)^i = -1 when i odd  */
            result -= (seq_t)1;
        }
    }
    return result;
}
