/*
 * A393818  a(n) is the least odd semiprime > 2*prime(n).
 * https://oeis.org/A393818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least odd semiprime > 2*prime(n) (offset 1)
 * Range:     u8 n <= 30, u16 n <= 3512, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

static inline bool is_semiprime_uint64(uint64_t n)
{
    /* odd semiprimes start at 9 = 3*3 */
    if (n < 9 || (n & 1ULL) == 0U) return false;

    for (uint64_t d = 3; d <= n / d; d += 2ULL) {
        if (n % d == 0ULL) {
            uint64_t q = n / d;
            /* q is <= sqrt(n), use big prime test for safety */
            if (is_prime_big(q)) return true;
            else return false;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* get the n-th prime (1‑based) */
    uint32_t pn = get_prime(n);
    uint64_t tgt = 2ULL * (uint64_t)pn;   /* 2 * prime(n) */
    uint64_t x = tgt + 1ULL;             /* start searching after tgt */
    if ((x & 1ULL) == 0ULL) ++x;         /* ensure odd candidate */

    for (;;) {
        if (is_semiprime_uint64(x))
            return (seq_t)x;
        x += 2ULL;
    }
}
