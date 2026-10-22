/*
 * A372785  a(n) = tau(5*n) = A000005(5*n).
 * https://oeis.org/A372785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(5*n) = divisor_count(5*n)
 * Range:     u8 n <= 9871, u16 n <= 9871, u32 n <= 9871, u64 n <= 9871,
 *            u128 n <= 9871, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computes the number of divisors of 5*n using a trial‑division factorisation.
   The function is fast enough for n up to about 10⁷ on a single call, which
   satisfies the 2‑second per call requirement for the test cases. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 5*n fits into 64 bits for all n used in the tests (n <= 10000). */
    uint64_t m = (uint64_t)5 * n;

    uint64_t div = 1;
    uint64_t tmp = m;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            div *= (exp + 1);
        }
    }
    if (tmp > 1)  /* remaining prime factor */
        div *= 2;

    return (seq_t)div;
}
