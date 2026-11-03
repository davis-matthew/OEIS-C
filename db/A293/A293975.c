/*
 * A293975  If n is even, divide by 2; otherwise, add the next larger prime.
 * https://oeis.org/A293975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/2 if n is even; otherwise a(n) = n + next_prime(n)
 * Range:     u8 n <= 126, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 85 terms through n = 9706 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (next_prime(n) returns the least prime strictly larger than n)      */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0)          /* n even */
        return (seq_t)(n / 2);
    else                          /* n odd */
    {
        uint64_t np = next_prime(n);   /* next prime > n */
        return (seq_t)n + (seq_t)np;
    }
}
