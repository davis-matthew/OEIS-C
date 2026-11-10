/*
 * A339092  Number of steps needed for a(1) = k ("the seed") to reappear in
 *          the sequence under the A339091 protocol, for k = 1, 2, 3, 4, ...
 *          to n. Else -1 if the seed never reappears.
 * https://oeis.org/A339092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   simulate the A339091 protocol with a(1)=k, then a(2)=1.. i.e.
 *            for i>=2: a(i)=sum of odd digits so far if i even, else sum of
 *            even digits so far; a(n)=first i>=2 with a(i)=k, else -1 (stop
 *            once both sums exceed k)
 * Range:     i8 n <= 52, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 98 terms through n = 9960 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;
    seq_t sumOdd = 0, sumEven = 0;
    seq_t x = k;
    while (x > 0) {
        seq_t d = x % 10;
        if ((d & 1) == 0) sumEven += d; else sumOdd += d;
        x /= 10;
    }
    for (idx_t i = 2; ; ++i) {
        if (sumOdd > k && sumEven > k) return (seq_t)(-1);
        seq_t cur = ((i & 1ULL) == 0ULL) ? sumOdd : sumEven;
        if (cur == k) return (seq_t)i;
        seq_t y = cur;
        while (y > 0) {
            seq_t d = y % 10;
            if ((d & 1) == 0) sumEven += d; else sumOdd += d;
            y /= 10;
        }
    }
}
