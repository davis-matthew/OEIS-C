/*
 * A253441  Number of (7+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(n-1)+588 for n>3; special values: a(1)=613,
 *            a(2)=612, a(3)=625
 * Range:     u8 none, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 613;
    if (n == 2) return 612;
    if (n == 3) return 625;

    /* compute 2^(n-1) */
    seq_t pow2 = 1;
    for (idx_t i = 1; i < n; ++i)
        pow2 *= 2;

    return pow2 * (seq_t)9 + (seq_t)588;
}
