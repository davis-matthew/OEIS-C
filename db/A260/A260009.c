/*
 * A260009  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000101 00010101 or 01010101.
 * https://oeis.org/A260009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 23, u64 n <= 51, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Empirical recurrence for OEIS A260009:
        a(1) = 49, a(2) = 98, a(3) = 252, a(4) = 578, a(5) = 1232,
        a(6) = 2738, a(7) = 6262, a(8) = 14112, a(9) = 31500,
        a(10) = 70688
   For n > 10:
        a(n) = 2*a(n-1) + a(n-3) + 3*a(n-4)
              - 5*a(n-5) - a(n-6) - a(n-8) + a(n-9)
   The function recomputes the values on each call (O(n) time, O(1) memory).
*/
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

seq_t a(idx_t n)
{
    assert(n >= 1);

    /* First ten terms (index 1..10).  Not a lookup table of further terms. */
    static const seq_t first10[10] = {
        49ULL, 98ULL, 252ULL, 578ULL, 1232ULL,
        2738ULL, 6262ULL, 14112ULL, 31500ULL, 70688ULL
    };

    if (n <= 10)
        return first10[n - 1];

    /* Ring buffer holding the last 10 computed values: indices 1..10 map to 0..9 */
    seq_t last[10];
    for (size_t i = 0; i < 10; ++i)
        last[i] = first10[i];

    seq_t val = 0;
    for (size_t i = 11; i <= (size_t)n; ++i) {
        /* indices for the recurrence */
        size_t idx  = (i - 1) % 10;   /* position to store a(i) */
        size_t i1   = (i - 2) % 10;   /* a(i-1) */
        size_t i3   = (i - 4) % 10;   /* a(i-3) */
        size_t i4   = (i - 5) % 10;   /* a(i-4) */
        size_t i5   = (i - 6) % 10;   /* a(i-5) */
        size_t i6   = (i - 7) % 10;   /* a(i-6) */
        size_t i8   = (i - 9) % 10;   /* a(i-8) */
        size_t i9   = (i -10) % 10;   /* a(i-9) */

        val = 2 * last[i1];
        val += last[i3];
        val += 3 * last[i4];
        val -= 5 * last[i5];
        val -= last[i6];
        val -= last[i8];
        val += last[i9];

        last[idx] = val;
    }

    return last[(n - 1) % 10];
}
