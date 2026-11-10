/*
 * A286054  Positions of 1 in A286052; complement of A286053.
 * https://oeis.org/A286054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A286052, the {101->0} transform
 *            of the Thue-Morse word; computed by streaming the transform and
 *            counting 1s
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9425 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned A286054_tm(idx_t k)
{
    return (unsigned)(__builtin_popcountll((unsigned long long)k) & 1u);
}

seq_t a(idx_t n)
{
    idx_t out = 1;
    idx_t in = 0;
    idx_t cnt = 0;

    while (1) {
        unsigned b0 = A286054_tm(in);
        unsigned e;
        if (b0 == 1 && A286054_tm(in + 1) == 0 && A286054_tm(in + 2) == 1) {
            e = 0;
            in += 3;
        } else {
            e = b0;
            in += 1;
        }
        if (e) {
            cnt++;
            if (cnt == n)
                return (seq_t)out;
        }
        out++;
    }
}
