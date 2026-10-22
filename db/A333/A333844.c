/*
 * A333844  G.f.: Sum_{k>=1} k * x^(k^4) / (1 - x^(k^4)).
 * https://oeis.org/A333844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^k‖n} (p^{⌊k/4⌋+1}−1)/(p−1) (multiplicative) (Also
 *            equal to sum of fourth roots of fourth powers dividing n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* harmless if called with 0 */
    seq_t result = 1;                /* multiplicative identity   */
    idx_t num = n;                   /* copy for factoring       */

    /* factor 2 separately */
    int exp = 0;
    while (num % 2 == 0) {
        num /= 2;
        exp++;
    }
    if (exp > 0) {
        int t = exp / 4;                  /* floor(exp/4)      */
        if (t > 0) {
            seq_t base = 2;
            seq_t pow = 1;
            for (int i = 0; i < t + 1; ++i)
                pow *= base;              /* 2^{t+1}           */
            pow -= 1;                      /* 2^{t+1}−1         */
            seq_t denom = base - 1;       /* 1                  */
            seq_t contrib = pow / denom;  /* (2^{t+1}−1)/(1)   */
            result *= contrib;
        }
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= num / p; p += 2) {
        if (num % p == 0) {
            int e = 0;
            while (num % p == 0) {
                num /= p;
                e++;
            }
            int t = e / 4;
            if (t > 0) {
                seq_t base = p;
                seq_t pow = 1;
                for (int i = 0; i < t + 1; ++i)
                    pow *= base;          /* p^{t+1}           */
                pow -= 1;                    /* p^{t+1}−1         */
                seq_t denom = base - 1;     /* p−1               */
                seq_t contrib = pow / denom;
                result *= contrib;
            }
        }
    }
    /* if num > 1 here, it is a prime with exponent 1 → contributes 1; ignored */

    return result;
}
