/*
 * A285976  Positions of 0 in A285975; complement of A285977.
 * https://oeis.org/A285976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285975 (the 00->0 transform of
 *            the Thue-Morse word); found by scanning the transformed word in
 *            O(n)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9386 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int A285977_tm(uint64_t i)
{
    return __builtin_popcountll((unsigned long long)i) & 1;
}

seq_t a(idx_t n)
{
    uint64_t i = 0;      /* index in Thue-Morse word (0-based) */
    uint64_t pos = 0;    /* position in transformed word */
    uint64_t zeros = 0;  /* zeros seen so far */

    if (n == 0)
        return 0;

    for (;;) {
        int b = A285977_tm(i);
        ++pos;
        if (b == 0) {
            ++zeros;
            if (zeros == (uint64_t)n)
                return (seq_t)pos;
            if (A285977_tm(i + 1) == 0)
                i += 2;
            else
                i += 1;
        } else {
            ++i;
        }
    }
}
