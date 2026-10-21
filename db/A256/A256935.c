/*
 * A256935  Concatenation of odd prime factors of numbers whose digits are all
 *          odd.
 * https://oeis.org/A256935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = concatenation of odd prime factors of the n‑th integer
 *            whose decimal digits are all odd (offset 1)
 * Range:     u8 n <= 11, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Test whether all decimal digits of x are odd */
static inline bool all_odd_digits(uint64_t x)
{
    if (x == 0) return false;
    while (x) {
        uint64_t d = x % 10;
        if ((d & 1) == 0) return false;
        x /= 10;
    }
    return true;
}

/* Return the n-th term of the sequence.  n>=1.  For n==1 the value
   is defined to be 0, because the first odd-digit number is 1, which
   has no prime factors. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;          /* special case for 1 */

    uint64_t target = 0;                 /* the n-th odd-digit number */
    uint64_t count  = 0;                 /* how many odd‑digit numbers seen */

    uint64_t num = 1;
    for (;;) {
        if (all_odd_digits(num)) {
            ++count;
            if (count == n) {
                target = num;
                break;
            }
        }
        ++num;
    }

    /* Factor target (which is odd) and build the concatenated result. */
    uint64_t tmp   = target;
    seq_t   result = 0;

    /* Trial division by odd numbers (primes). */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        while (tmp % p == 0) {
            seq_t prime = (seq_t)p;
            seq_t mult  = pow10((idx_t)digit_len(prime));
            result = result * mult + prime;
            tmp   /= p;
        }
    }
    /* Remaining prime factor (>1) */
    if (tmp > 1) {
        seq_t prime = (seq_t)tmp;
        seq_t mult  = pow10((idx_t)digit_len(prime));
        result = result * mult + prime;
    }

    return result;
}
