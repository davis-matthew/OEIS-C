/*
 * A263571  Expansion of f(x^2, x^2) * f(x, x^5) in powers of x where f(, ) is
 *          Ramanujan's general theta function.
 * https://oeis.org/A263571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(3*n + 1) where b(k) is multiplicative with *
 *            b(2^e)=(-1)^e, b(3^e)=(-1)^e, * b(p^e)=e+1 for p≡1,7 (mod 24), *
 *            b(p^e)=(e+1)(-1)^e for p≡5,11 (mod 24), * b(p^e)=(1+(-1)^e)/2
 *            for p≡13,17,19,23 (mod 24)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 3*n + 1 (always ≡1 mod 3, so 3 never divides m) */
    idx_t m = 3*n + 1;
    if (m == 1) return (seq_t)1;   /* b(1)=1 */

    long long prod = 1;            /* signed accumulator for multiplicative value */

    idx_t x = m;

    /* factor 2 */
    int e2 = 0;
    while ((x & 1) == 0) { e2++; x >>= 1; }
    if (e2) prod *= ((e2 & 1) ? -1 : 1);

    /* factor 3 is impossible for m=3*n+1, but skip just in case */
    int e3 = 0;
    while (x % 3 == 0) { e3++; x /= 3; }
    if (e3) prod *= ((e3 & 1) ? -1 : 1);

    /* factor remaining odd primes >3 */
    for (idx_t p = 5; p * p <= x; p += 2) {
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) { e++; x /= p; }

            long long val;
            int mod24 = (int)(p % 24);
            if (mod24 == 1 || mod24 == 7) {
                val = (long long)(e + 1);
            } else if (mod24 == 5 || mod24 == 11) {
                val = (long long)(e + 1) * ((e & 1) ? -1 : 1);
            } else { /* 13,17,19,23 */
                val = ((e & 1) ? 0 : 1);
            }
            prod *= val;
        }
    }

    /* possible leftover prime factor */
    if (x > 1) {
        int e = 1;
        long long val;
        int mod24 = (int)(x % 24);
        if (mod24 == 1 || mod24 == 7) {
            val = (long long)(e + 1);        /* 2 */
        } else if (mod24 == 5 || mod24 == 11) {
            val = (long long)(e + 1) * -1;   /* -2 */
        } else { /* 13,17,19,23 */
            val = 0;                         /* odd exponent -> 0 */
        }
        prod *= val;
    }

    /* prod should be non‑negative for all m ≡1 (mod 3) */
    return (seq_t)(prod >= 0 ? prod : 0);
}
