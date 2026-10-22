/*
 * A372504  Multiplicative with a(p^e) = e if e is a power of 2, and 0
 *          otherwise.
 * https://oeis.org/A372504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e | n} e if e is a power of 2, otherwise a(n)=0 *
 *            (multiplicative sequence A372504) * a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* return true if e is a power of two (e>0) */
static inline int is_power_of_two_uint64(uint64_t e)
{
    return e && ((e & (e - 1)) == 0);
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t result = 1;
    uint64_t nn = (uint64_t)n;

    /* handle factor 2 separately */
    if ((nn & 1ULL) == 0) {
        int exp = 0;
        while ((nn & 1ULL) == 0) {
            nn >>= 1;
            exp++;
        }
        if (!is_power_of_two_uint64((uint64_t)exp))
            return 0;
        result *= (seq_t)exp;
    }

    /* trial division by odd numbers */
    for (uint64_t p = 3; p <= nn / p; p += 2) {
        if (nn % p == 0) {
            int exp = 0;
            while (nn % p == 0) {
                nn /= p;
                exp++;
            }
            if (!is_power_of_two_uint64((uint64_t)exp))
                return 0;
            result *= (seq_t)exp;
        }
    }

    /* remaining prime factor, if any */
    if (nn > 1) {           /* nn itself is prime, exponent = 1 */
        /* 1 is a power of two, multiply by 1 (no effect) */
        /* result *= 1; // unnecessary */
    }

    return result;
}
