/*
 * A372601  The maximal exponent in the prime factorization of the largest
 *          exponentially odd divisor of n.
 * https://oeis.org/A372601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max{e_i - (e_i%2==0)} over all prime exponents e_i in n's
 *            factorization, with a(1)=0. This is equivalent to
 *            a(n)=A051903(A350390(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 2^64 fast trial division is inexpensive for n ≤ 10⁴ and comfortably fast for larger
    inputs when indexed up to about 10 000 calls.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)                     /* a(1) = 0 */
        return 0;

    uint64_t x = (uint64_t)n;       /* work on a 64‑bit copy */
    seq_t max_exp = 0;

    /* factor 2 first */
    if ((x & 1) == 0) {
        seq_t exp = 0;
        while ((x & 1) == 0) {      /* while x is even */
            x >>= 1;
            ++exp;
        }
        seq_t cand = (exp & 1) ? exp : exp - 1;
        if (cand > max_exp) max_exp = cand;
    }

    /* odd factors */
    for (uint64_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            seq_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            seq_t cand = (exp & 1) ? exp : exp - 1;
            if (cand > max_exp) max_exp = cand;
        }
    }

    /* remainder > 1 means a prime factor with exponent 1 */
    if (x > 1 && 1 > max_exp)
        max_exp = 1;

    return max_exp;
}
