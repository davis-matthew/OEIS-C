/*
 * A357905  a(n) = log_3(A060839(n)).
 * https://oeis.org/A357905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3-rank of (Z/nZ)* * computed by factoring n and counting
 *            * primes p≡1 (mod 3) once and 3^e with e≥2 once
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle small n */
    if (n < 3)        /* a(1)=0, a(2)=0, a(3)=0 */
        return 0;

    uint64_t temp = (uint64_t)n;
    seq_t rank = 0;

    /* factor 2 – contributes nothing */
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (p == 3) {
                if (e >= 2) ++rank;          /* 3^e with e≥2 contributes 1 */
            } else {
                if (p % 3 == 1) ++rank;      /* p≡1 (mod 3) contributes 1 */
            }
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (temp > 1) {
        /* temp is prime */
        uint64_t p = temp;
        if (p == 3) { /* e==1, does not contribute */ }
        else if (p % 3 == 1) ++rank;
    }

    return rank;
}
