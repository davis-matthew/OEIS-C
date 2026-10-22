/*
 * A350131  Number of segments lit to display the number n on a 9-segment
 *          display used by the Russian postal service.
 * https://oeis.org/A350131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ seg[d] over the decimal digits d of n, where seg =
 *            {6,3,4,4,4,5,5,3,7,5} for digits 0–9
 * Range:     u8 n <= 9199, u16 n <= 9199, u32 n <= 9199, u64 n <= 9199,
 *            u128 n <= 9199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t seg[10] = {6, 3, 4, 4, 4, 5, 5, 3, 7, 5};
    seq_t sum = 0;
    if (n == 0)  /* handle the single digit 0 explicitly */
        return seg[0];
    while (n != 0) {
        /* n % 10 yields an unsigned idx_t; cast to int for array index */
        int d = (int)(n % 10);
        sum += seg[d];
        n /= 10;
    }
    return sum;
}
