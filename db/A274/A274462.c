/*
 * A274462  Place n equally-spaced points around a circle, labeled
 *          0,1,2,...,n-1. For each i = 0..n-1 such that 4i != i mod n, draw
 *          an (undirected) chord from i to (4i mod n). Then a(n) is the total
 *          number of distinct chords.
 * https://oeis.org/A274462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) - (n%3==0?2:0) - ((n%15==0)?6 :
 *            ((n%15==5||n%15==10)?2:0)) ; a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t res = (seq_t)n - 1;
    if (n % 3 == 0) {
        if (res < 2) return 0;
        res -= 2;
    }
    if (n % 15 == 0) {
        if (res < 6) return 0;
        res -= 6;
    } else if (n % 15 == 5 || n % 15 == 10) {
        if (res < 2) return 0;
        res -= 2;
    }
    return res;
}
