/*
 * A301536  Number of n X 3 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally or vertically adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) -a(n-2) -a(n-3) +2*a(n-4) -a(n-5) +3*a(n-6) -
 *            11*a(n-7) -a(n-8) +2*a(n-9) -4*a(n-10) -2*a(n-11) -a(n-12)
 *            +11*a(n-13) + 3*a(n-14) +a(n-15) +2*a(n-16) +a(n-17) -a(n-18)
 *            -3*a(n-19) -a(n-20). Initial values: a(1)=3, a(2)=6, a(3)=9,
 *            a(4)=26, a(5)=61, a(6)=154, a(7)=374, a(8)=941, a(9)=...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Non‑incremental, strictly O(n) implementation. */
    static const seq_t init[20] = {
        3, 6, 9, 26, 61, 154, 374, 941, 2357, 5963,
        15074, 38168, 96641, 244836, 620311, 1571854,
        3983147, 10093850, 25579490, 64823479
    };

    if (n <= 20)
        return init[n - 1];

    /* Circular buffer for the last 20 terms of the recurrence. */
    seq_t buf[20];
    for (size_t i = 0; i < 20; ++i)
        buf[i] = init[i];

    size_t pos = 20;  /* points to the index that will contain a(n). */

    for (idx_t k = 21; k <= n; ++k) {
        /* Acquire recent values from the circular buffer. */
        seq_t xm1 = buf[(pos - 1) % 20];   /* a(n-1) */
        seq_t xm2 = buf[(pos - 2) % 20];   /* a(n-2) */
        seq_t xm3 = buf[(pos - 3) % 20];   /* a(n-3) */
        seq_t xm4 = buf[(pos - 4) % 20];   /* a(n-4) */
        seq_t xm5 = buf[(pos - 5) % 20];   /* a(n-5) */
        seq_t xm6 = buf[(pos - 6) % 20];   /* a(n-6) */
        seq_t xm7 = buf[(pos - 7) % 20];   /* a(n-7) */
        seq_t xm8 = buf[(pos - 8) % 20];   /* a(n-8) */
        seq_t xm9 = buf[(pos - 9) % 20];   /* a(n-9) */
        seq_t xm10 = buf[(pos - 10) % 20]; /* a(n-10) */
        seq_t xm11 = buf[(pos - 11) % 20]; /* a(n-11) */
        seq_t xm12 = buf[(pos - 12) % 20]; /* a(n-12) */
        seq_t xm13 = buf[(pos - 13) % 20]; /* a(n-13) */
        seq_t xm14 = buf[(pos - 14) % 20]; /* a(n-14) */
        seq_t xm15 = buf[(pos - 15) % 20]; /* a(n-15) */
        seq_t xm16 = buf[(pos - 16) % 20]; /* a(n-16) */
        seq_t xm17 = buf[(pos - 17) % 20]; /* a(n-17) */
        seq_t xm18 = buf[(pos - 18) % 20]; /* a(n-18) */
        seq_t xm19 = buf[(pos - 19) % 20]; /* a(n-19) */
        seq_t xm20 = buf[(pos - 20) % 20]; /* a(n-20) */

        seq_t next = 3 * xm1
                   - xm2
                   - xm3
                   + 2 * xm4
                   - xm5
                   + 3 * xm6
                   - 11 * xm7
                   - xm8
                   + 2 * xm9
                   - 4 * xm10
                   - 2 * xm11
                   - xm12
                   + 11 * xm13
                   + 3 * xm14
                   + xm15
                   + 2 * xm16
                   + xm17
                   - xm18
                   - 3 * xm19
                   - xm20;

        buf[pos % 20] = next;  /* store a(k) */
        ++pos;
    }

    return buf[(pos - 1) % 20];
}
