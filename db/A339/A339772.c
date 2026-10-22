/*
 * A339772  Number of Goldbach partitions of 2n whose parts have the same
 *          number of decimal digits.
 * https://oeis.org/A339772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..n} [floor(log10(k)) = floor(log10(2n-k))] *
 *            c(k) * c(2n-k) where c is the prime characteristic
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   digit_count, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>                 /* for uint64_t in digit_count argument */
#include "../utils/is_prime.h"
#include "../utils/digit_count.h"

/* Number of Goldbach partitions of 2n whose parts have the same number
   of decimal digits.   a(1) = 0, a(2) = 1, ... */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* offset starts at 1 */

    idx_t two_n = n + n;           /* 2 * n */
    seq_t count = 0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = two_n - k;       /* complementary part */

        /* both parts must be prime */
        if (!is_prime(k) || !is_prime(m))
            continue;

        /* same number of decimal digits */
        if (digit_count((uint64_t)k) == digit_count((uint64_t)m))
            count += 1;
    }
    return count;
}
