/*
 * A351654  Dirichlet g.f.: zeta(s) / (zeta(s-1) * zeta(s-2)).
 * https://oeis.org/A351654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 153, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A351654
 *  Dirichlet inverse of A069097.
 *  Sequence is multiplicative.
 *  a(p)   = -p^2 - p + 1
 *  a(p^e) = p^3 - p^2 - p + 1   (for e >= 2)
 *  a(1)   =  1
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined, but safe fallback */

    seq_t result = 1;
    idx_t temp = n;

    /* handle factor 2 separately */
    if ((temp & 1) == 0) {
        int e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        seq_t psq = (seq_t)2 * (seq_t)2;
        seq_t val;
        if (e == 1)
            val = -psq - (seq_t)2 + 1;
        else {
            seq_t pcu = psq * (seq_t)2;
            val = pcu - psq - (seq_t)2 + 1;
        }
        result *= val;
    }

    /* odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t psq = (seq_t)p * (seq_t)p;
            seq_t val;
            if (e == 1)
                val = -psq - (seq_t)p + 1;
            else {
                seq_t pcu = psq * (seq_t)p;
                val = pcu - psq - (seq_t)p + 1;
            }
            result *= val;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        seq_t p = temp;
        seq_t psq = p * p;
        seq_t val = -psq - p + 1;
        result *= val;
    }

    return result;
}
