/*
 * A282625  Number of cyclic groups in the total direct product factorization
 *          of the multiplicative group of integers modulo n, for n >= 1.
 * https://oeis.org/A282625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of cyclic components in the direct-product *
 *            factorization of the multiplicative group (Z/nZ)^× into cyclic
 *            groups * of prime‑power order
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* count distinct prime divisors of m (m>0) */
static size_t distinct_prime_count(uint64_t m)
{
    size_t cnt = 0;
    if ((m & 1ULL) == 0) {
        cnt++;
        while ((m & 1ULL) == 0)
            m >>= 1;
    }
    for (uint64_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            cnt++;
            while (m % d == 0)
                m /= d;
        }
    }
    if (m > 1)
        cnt++;
    return cnt;
}

seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n == 1 || n == 2)
        return (seq_t)1;

    seq_t result = 0;
    uint64_t temp = (uint64_t)n;

    /* factor out powers of 2 */
    if ((temp & 1ULL) == 0) {
        size_t e2 = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++e2;
        }
        if (e2 == 2)
            result += 1;          /* C2 */
        else if (e2 >= 3)
            result += 2;          /* C2 × C(2^(e2-2)) */
        /* e2 == 1: ignore the trivial factor for n > 2 */
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            size_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            size_t dcnt = distinct_prime_count(p - 1);
            seq_t contrib = (e == 1) ? (seq_t)dcnt
                                      : (seq_t)(1 + dcnt);
            result += contrib;
        }
    }

    /* remaining prime, if any */
    if (temp > 1) {
        /* temp is an odd prime > 2 */
        size_t dcnt = distinct_prime_count(temp - 1);
        result += (seq_t)dcnt;
    }

    return result;
}
