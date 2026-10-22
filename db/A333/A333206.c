/*
 * A333206  a(n) is the least decimal digit of n^3.
 * https://oeis.org/A333206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least decimal digit of n^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 4963, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0, whose cube is 0 with a single digit 0. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Compute n³.  The cast ensures the multiplication takes place in
       the seq_t type, which can be 8, 16, 32, 64, 128 bits or an
       arbitrary‑precision integer class. */
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;

    /* Find the minimum decimal digit of cube. */
    seq_t min_digit = (seq_t)9;  /* start with the largest possible digit */
    while (cube > (seq_t)0) {
        seq_t d = cube % (seq_t)10;   /* least significant decimal digit */
        if (d < min_digit)
            min_digit = d;
        cube /= (seq_t)10;            /* remove that digit */
    }

    return min_digit;
}
