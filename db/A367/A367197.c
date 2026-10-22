/*
 * A367197  Trace of addition table of group of divisors of n.
 * https://oeis.org/A367197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^v‖n} a(p^v) with * a(p^v) = (p^(v+1)-1)/(p-1) if v
 *            is even, * 2*(p^(v+1)-1)/(p^2-1) if v is odd
 * Range:     u8 n <= 16, u16 n <= 5673, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* input should be ≥1, safeguard */

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            seq_t prime = (seq_t)p;

            /* p^(exp+1) */
            seq_t powp = 1;
            for (uint64_t i = 0; i < exp + 1; ++i)
                powp *= prime;

            seq_t sigma = (powp - 1) / (prime - 1);

            seq_t local;
            if ((exp & 1ULL) == 0) {          /* even exponent */
                local = sigma;
            } else {                         /* odd exponent */
                seq_t numerator = 2 * sigma;
                seq_t denom     = prime + 1;
                local = numerator / denom;
            }

            result *= local;
        }
    }

    /* Remaining prime factor > sqrt(n) (exponent 1) */
    if (temp > 1) {
        seq_t prime = (seq_t)temp;          /* exponent = 1 (odd) */
        seq_t powp = prime * prime;              /* p^2 */
        seq_t sigma = (powp - 1) / (prime - 1);  /* p+1 */
        seq_t local = (2 * sigma) / (prime + 1);
        result *= local;
    }

    return result;
}
