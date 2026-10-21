/*
 * A258877  Primes p=prime(m) such that both p and m have the same digital
 *          root.
 * https://oeis.org/A258877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dr_equal, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine generates A258877: the n‑th prime p = prime(m) such that
   the digital root of p equals the digital root of its index m.  It
   iterates over the prime sequence using next_prime, checks the
   condition for each prime, and stops when the desired count is reached.
   The digital root of an integer x>0 is 1 + ((x‑1) mod 9), which works
   for any seq_t or uint64_t value. */
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/dr_equal.h"

seq_t a(idx_t n)
{
    if (n == 0)     return 0;       /* undefined, but return 0 */
    seq_t result = 0;
    idx_t  found  = 0;
    idx_t  num    = 0;           /* prime index (starting from 1) */
    uint64_t prev = 0;           /* previous prime, 0 to start with 2 */

    while (found < n) {
        prev = next_prime(prev);
        ++num;
        if (dr_equal(num, (seq_t)prev)) {
            ++found;
            if (found == n)  /* we've found the n‑th term */
                result = (seq_t)prev;
        }
    }
    return result;
}
