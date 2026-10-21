/*
 * A262284  Primes whose binary expansion begins 101.
 * https://oeis.org/A262284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Prime numbers whose binary representation starts with 101
 * Range:     u8 n <= 14, u16 n <= 1650, u32 n <= 9718, u64 n <= 9718,
 *            u128 n <= 9718, big any n
 * Helpers:   floor_log2_uint64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation generates primes with next_prime and tests the
   most significant three bits of each prime. */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/next_prime.h"
#include "../utils/floor_log2_uint64.h"

/* Return true if the binary representation of p starts with 101. */
static bool has_prefix_101(uint64_t p)
{
    if (p < 5) return false;
    idx_t len = floor_log2_uint64(p) + 1;   /* number of bits in p */
    uint64_t top = p >> (len - 3);
    return top == 5U;   /* 0b101 */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset, but safe fallback */

    idx_t count = 0;
    uint64_t p     = 5;            /* first prime that can match the pattern */

    for (;;)
    {
        if (has_prefix_101(p))
        {
            ++count;
            if (count == n) return (seq_t)p;
        }
        p = next_prime(p + 1);      /* advance to next prime */
    }
}
