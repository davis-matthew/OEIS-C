/*
 * A254614  Union of odd odious (A092246) and evil (A001969) numbers.
 * https://oeis.org/A254614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth nonnegative integer that is NOT an even odious number
 *            (i.e., a(n) is included if it is odd or has an even number of
 *            1‑bits)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   odd_bitcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/odd_bitcount.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t x = 0;          /* candidate integer */
    idx_t count = 0;      /* how many valid terms seen so far */

    while (true) {
        bool is_even = ((x & 1) == 0);
        bool oddcnt  = odd_bitcount(x);
        bool include = !(is_even && oddcnt);

        if (include) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
}
