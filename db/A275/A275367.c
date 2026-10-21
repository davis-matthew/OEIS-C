/*
 * A275367  Number of odd divisors of n^2.
 * https://oeis.org/A275367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) equals the number of odd divisors of n^2. Since n = 2^e0 *
 *            ∏_{p odd} p^{e_p}, the odd part of n^2 has each odd prime p
 *            raised to exponent 2e_p, hence a(n) = ∏_{p odd} (2*e_p + 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard */
    seq_t result = 1;              /* multiplicative identity */
    idx_t tmp = n;

    /* remove all factors of 2 – they do not affect odd divisors */
    while ((tmp & 1ULL) == 0)      /* while tmp is even */
        tmp >>= 1;                 /* divide by 2 */

    /* factor the remaining odd part by trial division */
    idx_t p = 3;
    while (p * p <= tmp) {
        int exp = 0;
        while (tmp % p == 0) {
            exp++;
            tmp /= p;
        }
        if (exp > 0)
            result *= (seq_t)(2 * exp + 1);  /* odd prime contribution */
        p += 2;
    }

    /* if a prime factor > sqrt remains, its exponent is 1 */
    if (tmp > 1)
        result *= 3;  /* 2*1+1 */

    return result;
}
