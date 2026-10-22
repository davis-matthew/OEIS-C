/*
 * A319205  a(n) = 1*2*3*4 + 5*6*7*8 + 9*10*11*12 + 13*14*15*16 + 17*18*19*20
 *          + ... + (up to n).
 * https://oeis.org/A319205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/4⌋} (4*i-3)*(4*i-2)*(4*i-1)*(4*i) + (if
 *            n%4>0) product_{k=0..(n%4)-1} (n-k)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 155, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;

    /* Full groups of four consecutive integers */
    idx_t full = n / 4;
    idx_t x = 1;
    for (idx_t i = 0; i < full; ++i) {
        seq_t prod = (seq_t)x * (seq_t)(x + 1) * (seq_t)(x + 2) * (seq_t)(x + 3);
        sum += prod;
        x += 4;
    }

    /* Remaining up to three trailing integers */
    idx_t rem = n % 4;
    if (rem) {
        seq_t prod = 1;
        for (idx_t k = 0; k < rem; ++k)
            prod *= (seq_t)(n - k);
        sum += prod;
    }

    return sum;
}
