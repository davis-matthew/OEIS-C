/*
 * A286636  Even numbers that are a sum of two squares plus 1.
 * https://oeis.org/A286636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth even integer such that (a(n)-1) is a sum of two
 *            squares
 * Range:     u8 n <= 43, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_sum_of_two_squares(uint64_t n)
{
    if (n == 0) return true;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if ((p & 3) == 3 && (exp & 1))
                return false;
        }
    }
    return !(m > 1 && (m & 3) == 3);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t count = 0;
    uint64_t candidate = 2;
    while (true) {
        if (is_sum_of_two_squares(candidate - 1)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 2;
    }
}
