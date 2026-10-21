/*
 * A276048  Sequence associated with the functional equation of the Riemann
 *          zeta zero spectrum (see formulas).
 * https://oeis.org/A276048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; if n=p^k (p prime) then a(n)=p^(p-1); otherwise a(n)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t seq_power(seq_t base, idx_t exp) {
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)0;

    /* Determine smallest prime divisor p of n */
    uint64_t m = n;
    uint64_t p = 0;

    if ((m & 1) == 0) {
        p = 2;
    } else {
        uint64_t i;
        for (i = 3; (uint64_t)i * i <= m; i += 2) {
            if (m % i == 0) {
                p = i;
                break;
            }
        }
        if (p == 0)  /* m is prime */
            p = m;
    }

    /* Check if n is a power of p */
    uint64_t temp = n;
    while (temp % p == 0)
        temp /= p;
    if (temp == 1) {
        /* n = p^k */
        return seq_power((seq_t)p, (idx_t)(p - 1));
    } else {
        return (seq_t)1;
    }
}
