/*
 * A377520  The sum of the divisors of n that are terms in A207481.
 * https://oeis.org/A377520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) multiplicative, a(p^e) = (p^(min(p,e)+1)-1)/(p-1)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS sequence A377520.                       */
#include <stdint.h>

static seq_t local_factor(uint64_t p, idx_t e)
{
    /* k = min(p,e)+1   */
    idx_t k = (p < e) ? (p + 1) : (e + 1);

    seq_t sum = 0;
    seq_t pow = 1;
    for (idx_t i = 0; i < k; ++i) {
        sum += pow;
        pow *= (seq_t)p;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t result = 1;
    idx_t tmp = n;

    /* factor 2 separately */
    if ((tmp & 1) == 0) {
        idx_t e = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++e;
        }
        result *= local_factor(2, e);
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            result *= local_factor(p, e);
        }
    }

    /* remaining prime factor > sqrt(tmp) */
    if (tmp > 1) {
        result *= local_factor(tmp, 1);
    }

    return result;
}
