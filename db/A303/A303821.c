/*
 * A303821  Number of ways to write 2*n as p + 2^x + 5^y, where p is a prime,
 *          and x and y are nonnegative integers.
 * https://oeis.org/A303821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 8129, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9833 (2026-11-01)
 * Helpers:   is_prime_seq, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the number of representations of 2*n as p + 2^x + 5^y
   where p is a prime and x,y ≥ 0.  The value is computed by brute
   force enumeration of all powers of 2 and 5 up to the target. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/pow2.h"

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Target value is 2*n. */
    seq_t target = (seq_t)n * (seq_t)2;

    seq_t count = 0;

    /* Enumerate powers of 2 up to target. */
    for (idx_t x = 0; ; ++x) {
        seq_t pow2x = pow2(x);
        if (pow2x > target) break;

        /* Enumerate powers of 5 up to target - pow2x. */
        for (idx_t y = 0; ; ++y) {
            seq_t pow5y = power5(y);
            seq_t sum = pow2x + pow5y;
            if (sum > target) break;

            seq_t p = target - sum;
            if (p <= (seq_t)1) continue;          /* p must be > 1 (prime). */

            if (is_prime_seq(p)) ++count;       /* count if p is prime. */
        }
    }

    return count;
}
