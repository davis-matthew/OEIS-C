/*
 * A241572  Numbers n such that 2*n+17 is not a prime.
 * https://oeis.org/A241572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that 2*n+17 is not a prime.
   a(n) = the n-th positive integer m for which 2*m+17 is composite. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Return the n-th integer m such that 2*m+17 is composite.
   If n == 0 the function returns 0 (undefined for the
   sequence which starts at offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: the OEIS offset is 1 */

    idx_t m = 1;                   /* candidate integer */
    idx_t cnt = 0;                 /* how many composites found so far */

    while (true) {
        seq_t val = (seq_t)2 * (seq_t)m + (seq_t)17;
        if (!is_prime_big(val)) {  /* 2*m+17 is not prime */
            ++cnt;
            if (cnt == n) {
                return (seq_t)m;   /* found the n-th composite */
            }
        }
        ++m;
    }
}
