/*
 * A267262  Total number of OFF (white) cells after n iterations of the "Rule
 *          111" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + (-1)^n * (n-4) + 2) / 2 for n > 3 * with initial
 *            values a(0)=0, a(1)=1, a(2)=3, a(3)=5
 * Range:     u8 n <= 15, u16 n <= 263, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 5;

    seq_t nseq = (seq_t)n;
    seq_t val;

    /* for n >= 4: use the closed formula */
    if ((n & 1ULL) == 0) {            /* n even:  + (n-4)  */
        val = nseq * nseq + (seq_t)(n - 4) + 2;
    } else {                          /* n odd:  - (n-4)  */
        val = nseq * nseq - (seq_t)(n - 4) + 2;
    }

    return val / 2;
}
