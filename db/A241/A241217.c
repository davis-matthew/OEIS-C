/*
 * A241217  Largest number that when multiplied by 7 produces an n-digit
 *          number.
 * https://oeis.org/A241217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(10^n / 7) (OEIS: A241217)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behavior for n < 1 is undefined */
        return 0;

    /* r holds 10^i mod 7; r is always in 0..6.  Start with i = 0: 10^0 = 1. */
    unsigned r = 1 % 7;     /* r = 1 */
    seq_t q = 0;            /* q = floor(10^i / 7), starting with i = 0 */

    /* Iterate i = 0 .. n-1; after each step q becomes floor(10^{i+1} / 7) */
    for (idx_t i = 0; i < n; ++i) {
        /* q_{i+1} = 10 * q_i + floor(10 * r / 7) */
        q = q * 10 + (seq_t)(10 * r / 7);
        /* update r for next iteration: r_{i+1} = (10 * r_i) mod 7 */
        r = (10 * r) % 7;
    }
    return q;
}
