/*
 * A347385  Dedekind psi function applied to the odd part of n: a(n) =
 *          A001615(A000265(n)).
 * https://oeis.org/A347385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Dedekind ψ of the odd part of n a(n) = ψ(A000265(n)),
 *            multiplicative: a(2^e)=1, a(p^e)= (p+1)*p^(e-1) for odd primes p
 * Range:     u8 n <= 164, u16 n <= 9844, u32 n <= 9844, u64 n <= 9844,
 *            u128 n <= 9844, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* odd part of n */
    uint64_t m = n;
    while ((m & 1ULL) == 0ULL) m >>= 1;

    seq_t res = 1;

    uint64_t temp = m;
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            /* compute p^(e-1) */
            seq_t pow = 1;
            for (int i = 0; i < e - 1; i++) {
                pow *= (seq_t)p;
            }
            seq_t contrib = (seq_t)(p + 1) * pow;
            res *= contrib;
        }
    }
    if (temp > 1) {          /* temp is a leftover odd prime factor */
        seq_t contrib = (seq_t)(temp + 1); /* p^(1-1)=1 */
        res *= contrib;
    }
    return res;
}
