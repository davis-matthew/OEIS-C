/*
 * A307420  Dirichlet g.f.: zeta(2*s) * zeta(3*s) / zeta(s).
 * https://oeis.org/A307420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = product over prime powers p^e of * 1 if e ≡ 0 or 2 (mod
 *            6) * -1 if e ≡ 1 (mod 6) * 0 otherwise * thus the sequence is
 *            multiplicative and takes values -1, 0, or 1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 1. */
    if (n == 0)            /* not used, but safe for completeness */
        return 0;
    if (n == 1)
        return 1;

    uint64_t tmp = (uint64_t)n;
    seq_t result = 1;      /* product of local values */

    /* Process the prime factor 2 separately for speed. */
    if ((tmp & 1ULL) == 0) {
        int e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            ++e;
        }
        int mod = e % 6;
        seq_t val;
        if (mod == 0 || mod == 2)
            val = 1;
        else if (mod == 1)
            val = -1;
        else        /* mod in {3,4,5} */
            val = 0;

        if (val == 0)
            return 0;
        result *= val;
        if (result == 0)
            return 0;
    }

    /* Odd primes.  We iterate up to sqrt(original n) to avoid overflow. */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            int mod = e % 6;
            seq_t val;
            if (mod == 0 || mod == 2)
                val = 1;
            else if (mod == 1)
                val = -1;
            else
                val = 0;

            if (val == 0)
                return 0;
            result *= val;
            if (result == 0)
                return 0;
        }
    }

    /* Any remaining prime factor has exponent 1. */
    if (tmp > 1) {
        /* exponent e = 1 => e % 6 == 1 => val = -1 */
        result *= (seq_t)(-1);
    }

    return result;
}
