/*
 * A384667  Characteristic function of powers of squarefree numbers (A072774).
 * https://oeis.org/A384667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if all exponents in the prime factorization of n are
 *            equal, 0 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1)=1, and n=0 is harmless */

    uint64_t cur = n;
    seq_t first_exp = 0;
    seq_t exp;
    int seen = 0;

    /* factor 2 */
    exp = 0;
    while ((cur & 1ULL) == 0) {
        cur >>= 1;
        exp++;
    }
    if (exp) {
        first_exp = exp;
        seen = 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= cur / p; p += 2) {
        exp = 0;
        while (cur % p == 0) {
            cur /= p;
            exp++;
        }
        if (exp) {
            if (!seen) {
                first_exp = exp;
                seen = 1;
            } else if (exp != first_exp) {
                return 0;
            }
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (cur > 1) {
        exp = 1;
        if (!seen) {
            first_exp = exp;
            seen = 1;
        } else if (exp != first_exp) {
            return 0;
        }
    }

    return 1;
}
