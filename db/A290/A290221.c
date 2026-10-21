/*
 * A290221  Number of elements added at n-th stage to the structure of the
 *          narrow cross described in A290220.
 * https://oeis.org/A290221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(0..6) = 0,2,4,4,8,8,8; for n>=7, a(n) = [8,16,12][(n-7)%3]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 7)
    {
        static const seq_t init[7] = {0, 2, 4, 4, 8, 8, 8};
        return init[n];
    }
    else
    {
        unsigned r = (unsigned)((n - 7) % 3);
        switch (r)
        {
            case 0: return 8;
            case 1: return 16;
            default: return 12;
        }
    }
}
