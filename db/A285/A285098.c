/*
 * A285098  Row sums of irregular triangle A070168.
 * https://oeis.org/A285098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the numbers in the Terras-modified Collatz
 *            trajectory starting at n
 * Range:     u8 n <= 14, u16 n <= 326, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The trajectory follows: if x is even, next = x/2; otherwise next = (3*x+1)/2. The sum includes the starting n and the final 1. */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t cur  = (seq_t)n;
    while (1) {
        sum += cur;
        if (cur == 1) {
            break;
        }
        if ((cur & 1) == 0) {
            cur = cur / 2;
        } else {
            cur = (cur * 3 + 1) / 2;
        }
    }
    return sum;
}
