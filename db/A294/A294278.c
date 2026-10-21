/*
 * A294278  Numbers k such that omega(k) > omega(k+1) (where omega(m) =
 *          A001221(m), the number of distinct primes dividing m).
 * https://oeis.org/A294278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = k such that omega(k) > omega(k+1) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

/* Return the n-th term of A294278 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined for n==0, return 0 */

    idx_t found = 0;
    uint64_t k = 1;          /* natural numbers start at 1 */

    while (1) {
        uint64_t w1 = count_distinct_primes(k);
        uint64_t w2 = count_distinct_primes(k + 1);

        if (w1 > w2) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }
}
