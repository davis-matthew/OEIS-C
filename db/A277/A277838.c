/*
 * A277838  Number of '8' digits in the set of all numbers from 0 to
 *          A014824(n) = Sum_{i=1..n} i*10^(n-i) = (0, 1, 12, 123, 1234,
 *          12345, ...).
 * https://oeis.org/A277838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digit '8' occurrences in all integers from 0 up
 *            to A014824(n) = ∑_{i=1}^n i·10^(n-i). A014824(n) is computed
 *            iteratively as N_{0}=0, N_{k}=N_{k-1}·10 + k. The digit count is
 *            obtained with the standard per‑position formula for counting
 *            occurrences of a digit in the range [0,N]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Build N = A014824(n) as a big integer (or 64‑bit integer). */
    seq_t N = 0;
    for (idx_t i = 1; i <= n; ++i) {
        N = N * 10 + i;
    }

    /* Count digit '8' in the range 0..N. */
    seq_t count = 0;
    seq_t pow10 = 1;
    while (pow10 <= N) {
        /* high = ⌊N / (pow10·10)⌋, cur = digit at this place, low = N mod pow10 */
        seq_t high, cur, low;
        if (pow10 <= N / 10) {
            high = N / (pow10 * 10);
        } else {
            high = 0;
        }
        cur = (N / pow10) % 10;
        low = N % pow10;

        count += high * pow10;          /* base occurrences from higher blocks */
        if (cur > 8) {
            count += pow10;             /* entire lower block */
        } else if (cur == 8) {
            count += low + 1;           /* partial block ending at N */
        }
        pow10 *= 10;                    /* move to the next higher power of 10 */
    }
    return count;
}
