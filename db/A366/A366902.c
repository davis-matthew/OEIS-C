/*
 * A366902  The number of exponentially evil divisors of n.
 * https://oeis.org/A366902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e || n} (# of evil numbers ≤ e)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9898 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (An exponentially evil divisor is a divisor whose prime exponents
   are all evil numbers – numbers with an even number of 1‑bits
   in binary.  A(n) counts such divisors.) */

#include <stdint.h>
#include <stddef.h>

/* Count evil numbers in the range [0, e] inclusive. */
static seq_t count_evil_upto(uint64_t e)
{
    seq_t cnt = 0;
    for (uint64_t i = 0; i <= e; ++i) {
        /* __builtin_parityll returns 1 if popcount(i) is odd. */
        if (__builtin_parityll(i) == 0)
            ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but keep sane */

    uint64_t temp = (uint64_t)n;
    seq_t result = 1;

    /* Trial division factorisation of n. */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result *= count_evil_upto(exp);
        }
    }
    /* If a prime factor > sqrt(n) remains. */
    if (temp > 1) {
        result *= count_evil_upto(1);   // exponent is 1
    }

    return result;
}
