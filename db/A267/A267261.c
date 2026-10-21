/*
 * A267261  Number of OFF (white) cells in the n-th iteration of the "Rule
 *          111" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n > 4, a(n) = { 4 if n is odd, 2*n-5 if n is even } a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=2, a(4)=4
 * Range:     u8 n <= 131, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
    case 0ULL: return 0;
    case 1ULL: return 1;
    case 2ULL: return 2;
    case 3ULL: return 2;
    case 4ULL: return 4;
    default:
        if ((n & 1ULL) == 0ULL)   /* even */
            return (seq_t)((seq_t)2 * (seq_t)n - (seq_t)5);
        else                     /* odd */
            return (seq_t)4;
    }
}
