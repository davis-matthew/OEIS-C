/*
 * A275102  Number of set partitions of [5*n] such that within each block the
 *          numbers of elements from all residue classes modulo n are equal
 *          for n>0, a(0)=1.
 * https://oeis.org/A275102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 246*a(n-1) - 20545*a(n-2) + 751800*a(n-3) -
 *            12911500*a(n-4) + 100380000*a(n-5) - 304200000*a(n-6) +
 *            216000000*a(n-7) for n >= 8 Initial values: a(0)=1, a(1)=52,
 *            a(2)=1496, a(3)=69026, a(4)=4383626, a(5)=350813126,
 *            a(6)=33056715626, a(7)=3464129078126
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 8 terms */
    static const seq_t init[8] = {
        (seq_t)1ULL,
        (seq_t)52ULL,
        (seq_t)1496ULL,
        (seq_t)69026ULL,
        (seq_t)4383626ULL,
        (seq_t)350813126ULL,
        (seq_t)33056715626ULL,
        (seq_t)3464129078126ULL
    };

    if (n < 8) return init[n];

    /* Sliding window of the last 7 values */
    seq_t win[8];
    for (idx_t i = 0; i < 8; ++i) win[i] = init[i];

    for (idx_t i = 8; i <= n; ++i)
    {
        /* recurrence: a[i] = 246*a[i-1] - 20545*a[i-2] + 751800*a[i-3]
                          - 12911500*a[i-4] + 100380000*a[i-5]
                          - 304200000*a[i-6] + 216000000*a[i-7] */
        seq_t next = (seq_t)246ULL * win[(i-1) % 8]
                   - (seq_t)20545ULL * win[(i-2) % 8]
                   + (seq_t)751800ULL * win[(i-3) % 8]
                   - (seq_t)12911500ULL * win[(i-4) % 8]
                   + (seq_t)100380000ULL * win[(i-5) % 8]
                   - (seq_t)304200000ULL * win[(i-6) % 8]
                   + (seq_t)216000000ULL * win[(i-7) % 8];

        win[i % 8] = next;
    }

    return win[n % 8];
}
