/*
 * A365589  Numbers that have at least one prime digit and at least one
 *          nonprime digit.
 * https://oeis.org/A365589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer whose decimal representation contains at least one
   prime digit (2,3,5,7) and at least one nonprime digit (0,1,4,6,8,9). */
#include <stdbool.h>

static bool has_prime_and_nonprime(idx_t x)
{
    bool hasPrime   = false;
    bool hasNonPrime = false;
    while (x > 0) {
        unsigned d = x % 10;
        if (d == 2 || d == 3 || d == 5 || d == 7)
            hasPrime = true;
        else
            hasNonPrime = true;
        if (hasPrime && hasNonPrime)
            return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          // undefined behavior for n<1; return 0
    idx_t count = 0;
    idx_t cand   = 12;            // first number with both prime and nonprime digits
    while (1) {
        if (has_prime_and_nonprime(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}
