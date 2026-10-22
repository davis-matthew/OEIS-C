/*
 * A358098  a(n) is the largest integer m < n such that m and n have no common
 *          digit, or -1 when such integer m does not exist.
 * https://oeis.org/A358098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest m < n with no common decimal digit to n; *
 *            returns -1 if no such m exists. 0 is treated as having digit 0
 * Range:     u8 n <= 298, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return a 10‑bit mask: bit i is set if digit i occurs in x.
   The digit 0 is represented by bit 0; for x==0 the mask is 1. */
static inline unsigned digit_mask(seq_t x)
{
    if (x == 0) return 1u;          /* only digit 0 */
    unsigned m = 0;
    while (x > 0) {
        seq_t dseq = x % 10;        /* digit as seq_t */
        unsigned d = (unsigned)dseq;
        m |= 1u << d;
        x = x / 10;
    }
    return m;
}

seq_t a(idx_t n)
{
    unsigned mask_n = digit_mask((seq_t)n);
    idx_t m = n - 1;                /* start with the largest candidate */
    for (;;) {
        unsigned mask_m = digit_mask((seq_t)m);
        if ((mask_n & mask_m) == 0)          /* no common digit */
            return (seq_t)m;
        if (m == 0)                         /* reached 0, stop */
            break;
        --m;
    }
    return (seq_t)-1;            /* no m found */
}
