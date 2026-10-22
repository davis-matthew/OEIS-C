/*
 * A324893  a(n) = sigma(A097706(n)), where A097706(n) is the part of n
 *          composed of prime factors of form 4k+3.
 * https://oeis.org/A324893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(A097706(n)) A097706(n) is the part of n composed of
 *            prime factors ≡ 3 (mod 4). a(n) is multiplicative with a(p^e) =
 *            (p^(e+1)-1)/(p-1) for p ≡ 3 (mod 4) and 1 otherwise
 * Range:     u8 n <= 48, u16 n <= 7327, u32 n <= 7327, u64 n <= 7327,
 *            u128 n <= 7327, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;      /* a(1) = 1 */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* factor 2 – it is not of the form 4k+3, so we just remove it */
    if ((m & 1) == 0) {
        while ((m & 1) == 0) m >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }

            if (p % 4 == 3) {
                seq_t pw = (seq_t)1;
                for (int i = 0; i < e; ++i) pw *= (seq_t)p;     /* p^e   */
                seq_t pw_next = pw * (seq_t)p;                  /* p^(e+1) */
                seq_t numerator = pw_next - (seq_t)1;          /* p^(e+1)-1 */
                seq_t denom = (seq_t)(p - 1);                  /* p-1   */
                seq_t term = numerator / denom;               /* sigma(p^e) */
                result *= term;
            }
        }
    }

    /* remaining prime factor, if any */
    if (m > 1 && (m % 4 == 3)) {
        seq_t term = (seq_t)(m + 1);   /* sigma(m) for e = 1 */
        result *= term;
    }

    return result;
}
