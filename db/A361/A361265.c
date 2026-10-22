/*
 * A361265  Multiplicative with a(p^e) = e * p^(e + 1), e > 0.
 * https://oeis.org/A361265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} e * p^(e+1) (multiplicative function)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Simple exponentiation by squaring for seq_t. */
static seq_t seq_pow_int(seq_t base, int exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;            /* a(1) = 1 */

    uint64_t temp = (uint64_t)n;
    seq_t res = 1;

    /* Factor out powers of 2 */
    int e = 0;
    while (!(temp & 1ULL)) {       /* temp % 2 == 0 */
        temp >>= 1;
        e++;
    }
    if (e > 0) {
        seq_t pe = seq_pow_int((seq_t)2, e + 1);
        res *= (seq_t)e * pe;
    }

    /* Factor odd primes */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        if (e > 0) {
            seq_t pe = seq_pow_int((seq_t)p, e + 1);
            res *= (seq_t)e * pe;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {                /* e == 1 */
        seq_t pe = seq_pow_int((seq_t)temp, 2); /* p^(1+1) */
        res *= pe;                /* multiply by 1 * pe */
    }

    return res;
}
