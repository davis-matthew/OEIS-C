/*
 * A384511  a(n) is the number of ways to partition n X n X n cube into five
 *          distinct cuboids with three full-length axial spanning parts
 *          sharing only two cube corners each.
 * https://oeis.org/A384511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For odd n, a(n)=n*(n-1)*(n-2)/6. For even n,
 *            a(n)=n*(n-1)*(n-2)/6 - (n/2 - 1)
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t tn = (seq_t)n;
    /* Compute binomial coefficient C(n,3) safely in 64‑bit. */
    uint64_t prod = (uint64_t)tn * (uint64_t)(tn - 1) * (uint64_t)(tn - 2);
    seq_t c = (seq_t)(prod / 6u);

    if ((n & 1) == 0) {          /* even n */
        seq_t diff = (seq_t)(n / 2) - 1;
        return c - diff;
    } else {                     /* odd n */
        return c;
    }
}
