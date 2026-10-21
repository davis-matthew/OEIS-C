/*
 * A289927  p-INVERT of A014217 (starting at n=1), where p(S) = 1 - S - S^2.
 * https://oeis.org/A289927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(0)=1, a(1)=4, a(2)=15, a(3)=53, a(4)=187, a(5)=656, a(6)=2301, a(7)=8071,
 *           a(n)=3*a(n-1)+4*a(n-2)-7*a(n-3)-5*a(n-4)+7*a(n-5)+4*a(n-6)-3*a(n-7)-a(n-8) for n>7
 */

#include <stddef.h>

/* External linkage as required */
seq_t a(idx_t n) {
    /* Base values for n = 0 … 7, copied from the OEIS entry. */
    static const seq_t base[8] = { 1, 4, 15, 53, 187, 656, 2301, 8071 };

    /* Return early for the first eight terms. */
    if (n <= 7)
        return base[(size_t)n];

    /* Use a circular buffer of the last eight terms to avoid large memory usage
       while still staying within the limits of seq_t for all indices that fit. */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = base[i];

    /* Compute from n = 8 up to the requested index. */
    for (idx_t i = 8; i <= n; ++i) {
        /* Indices in the buffer are taken modulo 8. */
        size_t m1 = (i - 1) & 7;
        size_t m2 = (i - 2) & 7;
        size_t m3 = (i - 3) & 7;
        size_t m4 = (i - 4) & 7;
        size_t m5 = (i - 5) & 7;
        size_t m6 = (i - 6) & 7;
        size_t m7 = (i - 7) & 7;
        size_t m8 = (i - 8) & 7;

        seq_t pos = 3 * buf[m1] + 4 * buf[m2] + 7 * buf[m5] + 4 * buf[m6];
        seq_t neg = 7 * buf[m3] + 5 * buf[m4] + 3 * buf[m7] +     buf[m8];

        buf[i & 7] = pos - neg;   /* pos >= neg guarantees a non‑negative result. */
    }

    return buf[n & 7];
}
