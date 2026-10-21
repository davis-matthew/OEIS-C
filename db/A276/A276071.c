/*
 * A276071  n^3 followed by n^2 followed by n^4 followed by n.
 * https://oeis.org/A276071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = if n % 4 == 0: (n/4)^3 else if n % 4 == 1: ((n-1)/4)^2
 *            else if n % 4 == 2: ((n-2)/4)^4 else: ((n-3)/4)
 * Range:     u8 n <= 17, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 4;
    int r = n % 4;
    seq_t base = (seq_t)k;

    switch (r)
    {
        case 0:
            /* (n/4)^3 */
            return base * base * base;
        case 1:
            /* ((n-1)/4)^2 */
            return base * base;
        case 2:
            /* ((n-2)/4)^4 */
            return base * base * base * base;
        case 3:
            /* ((n-3)/4) */
            return (seq_t)k;
        default:
            return 0;
    }
}
