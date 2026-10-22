/*
 * A368105  The number of bi-unitary divisors of n that are powerful
 *          (A001694).
 * https://oeis.org/A368105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(e) where f(e) = e if e == 2 or e is odd; e-1
 *            otherwise. Multiplicative, a(1)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety – OEIS offset starts at 1 */

    seq_t result = 1;
    idx_t m = n;

    /* factor 2 separately */
    if ((m & 1ULL) == 0) {
        idx_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;           /* m /= 2 */
            ++e;
        }
        seq_t term = (e == 2 || (e & 1ULL)) ? (seq_t)(e) : (seq_t)(e - 1);
        result *= term;
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t term = (e == 2 || (e & 1ULL)) ? (seq_t)(e) : (seq_t)(e - 1);
            result *= term;
        }
    }

    /* remaining prime factor (if any) has exponent 1, term = 1 */
    if (m > 1) {
        /* term = 1 (odd exponent) – multiplication is a no‑op
           but kept for explicitness */
        result *= (seq_t)1;
    }

    return result;
}
