/*
 * A381951  Nonsquarefree odd numbers whose prime factorization has an odd
 *          maximum exponent.
 * https://oeis.org/A381951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1251, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A381951
 *  a(n) = the n-th odd integer that is not squarefree and whose maximum
 *         prime exponent in its factorisation is odd (and ≥3).
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: test whether an odd number k meets the conditions */
static bool qualifies(seq_t k)
{
    /* must be odd */
    if (((seq_t)k & 1) == 0)
        return false;

    /* For the algorithm we work with 64‑bit integers.
       The values we generate for n up to about 10⁴ are safely within that range. */
    uint64_t n = (uint64_t)k;
    uint64_t tmp = n;
    uint64_t max_exp = 0;
    uint64_t exp;

    /* check factor 3 onward (2 cannot divide an odd number) */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            if (exp > max_exp)
                max_exp = exp;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (tmp > 1) {
        exp = 1;    /* exponent of the remaining prime is 1 */
        if (exp > max_exp)
            max_exp = exp;
    }

    /* not squarefree (max_exp ≥ 2) and the maximum exponent odd */
    return (max_exp >= 2) && ((max_exp & 1) == 1);
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)   /* not defined for 0, guard anyway */
        return 0;

    seq_t count = 0;
    seq_t k = 1;              /* start from the first odd > 1 */

    for (;;) {
        k += 2;               /* only odd candidates */
        if (qualifies(k)) {
            count++;
            if (count == n)
                return k;
        }
    }
}
