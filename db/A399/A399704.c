/*
 * A399704  a(n) is the smallest integer >= n such that (n-1)*n/2 + a(n)
 *          divides (n-1)! * a(n).
 * https://oeis.org/A399704
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest integer >= n such that ((n-1)*n)/2 + a(n)
 *            divides (n-1)! * a(n)
 * Range:     u8 n <= 26, u16 n <= 3244, u32 n <= 6386, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for n < 3 – not required to be correct. */
    if (n < 3)
        return 0;

    /* Calculate the constant part T = (n-1)*n/2 as seq_t. */
    seq_t T = (seq_t)n * (seq_t)(n - 1) / 2;

    for (seq_t a_val = (seq_t)n; ; ++a_val) {
        seq_t S = T + a_val;          /* S = T + a_val */
        seq_t mod = 1;                /* (n-1)! modulo S */

        /* Compute (n-1)! mod S. */
        for (idx_t i = 2; i < n; ++i) {
            mod = (mod * (seq_t)i) % S;
        }

        /* Check if S divides (n-1)! * a_val. */
        if ((mod * a_val) % S == 0) {
            return a_val;
        }
    }
}
