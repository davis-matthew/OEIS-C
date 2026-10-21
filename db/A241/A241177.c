/*
 * A241177  Numbers n such that there are exactly two numbers m with m + (some
 *          digit of m) = n.
 * https://oeis.org/A241177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 3950, u32 n <= 9677, u64 n <= 9677,
 *            u128 n <= 9677, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A241177.  The nth term is the nth integer > 0 that has
   exactly two representations n = m + d where d is a digit of m.
   The routine counts such representations by iterating over n and testing
   all possible digits d = 0…9. */

#include <stdint.h>
#include <stdbool.h>

/* Return true iff digit d occurs in the decimal representation of num. */
static bool contains_digit(idx_t num, int d) {
    if (num == 0)
        return d == 0;
    while (num > 0) {
        if (num % 10 == d)
            return true;
        num /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0)   /* undefined below offset */
        return 0;

    idx_t found = 0;          /* how many qualifying numbers seen */
    for (idx_t num = 10; ; ++num) {
        int cnt = 0;          /* count of valid representations for this num */

        for (int d = 0; d <= 9; ++d) {
            if (num < (idx_t)d)
                continue;     /* m = num - d would be negative */

            idx_t m = num - (idx_t)d;

            if (contains_digit(m, d))
                ++cnt;

            if (cnt > 2)
                break;        /* no need to continue */
        }

        if (cnt == 2) {
            ++found;
            if (found == n)
                return (seq_t)num;
        }
    }
}
