/*
 * A253461  Number of (n+1) X (n+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal maximum minus antidiagonal minimum unequal to its
 *          neighbors horizontally, vertically, diagonally and antidiagonally.
 * https://oeis.org/A253461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-4) for n > 5, with initial values: a(1)=81,
 *            a(2)=800, a(3)=2000, a(4)=3744, a(5)=8000
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[5] = {
        (seq_t)81,
        (seq_t)800,
        (seq_t)2000,
        (seq_t)3744,
        (seq_t)8000
    };

    if (n <= 5)
        return init[n - 1];

    /* For n >= 6 use a(n) = 16 * a(n-4) recursively. */
    idx_t exponent = ((n - 6) / 4) + 1;      /* number of times to apply the factor 16   */
    idx_t base_index = n - 4 * exponent;     /* 1 <= base_index <= 5                      */

    seq_t result = init[base_index - 1];

    /* Multiply by 16^exponent */
    for (idx_t i = 0; i < exponent; ++i)
        result *= (seq_t)16;

    return result;
}
