/*
 * A247343  Moebius transform applied four times to sequence 1,0,0,0,....
 * https://oeis.org/A247343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   not stated
 * Range:     i8 n <= 179, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247343: multiplicative with a(p^e) = (-1)^e * binomial(4, e) */

/* Expected use: a(1) = 1, a(2) = -4, a(3) = -4, a(4) = 6, ... */

seq_t a(idx_t n)
{
    /* For n >= 1; no special handling for 0 is needed. */
    seq_t res = 1;
    uint64_t temp = n;                   /* temp is 64 bit for factorisation      */
    const seq_t binom[5] = {1, 4, 6, 4, 1};

    /* Handle factor 2 separately and avoid test p*p <= temp for every p */
    uint64_t e = 0;
    while ((temp & 1ULL) == 0ULL) {
        temp >>= 1;
        e++;
    }
    if (e > 4) {
        return 0;
    }
    if (e) {  /* e may be 0, in which case skip */
        seq_t m = (e & 1ULL) ? -binom[e] : binom[e];
        res *= m;
    }

    /* Factor odd primes 3,5,7,... */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            if (e > 4) {
                return 0;
            }
            seq_t m = (e & 1ULL) ? -binom[e] : binom[e];
            res *= m;
        }
    }

    /* If anything left, it's a prime factor with exponent 1 */
    if (temp > 1) {
        e = 1;
        seq_t m = (e & 1ULL) ? -binom[e] : binom[e];   /* here m = -4 */
        res *= m;
    }

    return res;
}
