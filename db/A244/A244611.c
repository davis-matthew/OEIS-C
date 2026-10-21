/*
 * A244611  Expansion of (phi(q) + phi(q^2) - phi(q^3) - phi(q^6)) / 2 in
 *          powers of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A244611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   multiplicative with a(2^e)=1, a(3^e)=(-1)^e, a(p^e)=
 *            (1+(-1)^e)/2 for p>3. Thus a(n) ∈ {−1,0,1}
 * Range:     i8 n <= 9731, i16 n <= 9731, i32 n <= 9731, i64 n <= 9731,
 *            i128 n <= 9731, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) /* a(1) = 1 */
        return 1;

    seq_t result = 1;          /* multiplicative identity */
    idx_t temp = n;

    /* remove all factors of 2 – they contribute multiplicatively 1 */
    while ((temp & 1) == 0) { /* temp % 2 == 0 */
        temp >>= 1;           /* temp /= 2; */
    }

    /* factor odd primes up to sqrt(temp) */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p != 0)
            continue;

        /* count exponent of p */
        unsigned exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }

        if (p == 3) {
            /* multiply by (-1)^exp */
            if (exp & 1)
                result = -result;
        } else {
            /* p > 3: if exp odd, a(p^exp)=0 -> overall 0 */
            if (exp & 1)
                return 0;
            /* otherwise even exponent contributes 1, do nothing */
        }
    }

    /* if something remains, it is a prime > sqrt(original n) */
    if (temp > 1) {
        /* temp is prime */
        if (temp == 3) {
            result = -result;
        } else {
            /* odd exponent 1 for prime > 3 => return 0 */
            return 0;
        }
    }

    return result;
}
