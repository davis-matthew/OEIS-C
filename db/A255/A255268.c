/*
 * A255268  a(n) = Product_{k=1..n} k!^n.
 * https://oeis.org/A255268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{k=1..n} k!)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sf = 1;   /* superfactorial: ∏_{k=1..n} k!   */
    seq_t fac = 1;  /* current factorial k!          */
    for (idx_t k = 1; k <= n; ++k) {
        fac *= (seq_t)k;   /* fac = k!   */
        sf  *= fac;        /* sf  = sf * k!  */
    }
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i)
        result *= sf;      /* a(n) = sf^n */
    return result;
}
