/*
 * A291725  p-INVERT of (1,0,1,0,0,0,0,...), where p(S) = (1 - S)^2.
 * https://oeis.org/A291725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 49, u64 n <= 105, u128 n <= 219,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A291725: p-INVERT of (1,0,1,0,0,0,0,…) with p(S)=(1‑S)².
 * Recurrence (for n ≥ 7):
 *   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-4) - a(n-6)
 * Initial values:
 *   a(0)=2, a(1)=3, a(2)=6, a(3)=11, a(4)=18, a(5)=30, a(6)=50
 */
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Base values for n < 7 */
    static const seq_t base[7] = {
        (seq_t)2, (seq_t)3, (seq_t)6, (seq_t)11, (seq_t)18, (seq_t)30, (seq_t)50
    };

    if (n < 7) return base[n];

    /* ring buffer of the last seven computed terms */
    seq_t buf[7];
    for (size_t k = 0; k < 7; ++k) buf[k] = base[k];

    for (idx_t i = 7; i <= n; ++i) {
        size_t j1 = (size_t)(i - 1) % 7;
        size_t j2 = (size_t)(i - 2) % 7;
        size_t j3 = (size_t)(i - 3) % 7;
        size_t j4 = (size_t)(i - 4) % 7;
        size_t j6 = (size_t)(i - 6) % 7;
        /* Compute a(i) using the recurrence */
        seq_t next = 2 * buf[j1]
                     - buf[j2]
                     + 2 * buf[j3]
                     - 2 * buf[j4]
                     - buf[j6];
        buf[(size_t)i % 7] = next;
    }

    return buf[(size_t)n % 7];
}
