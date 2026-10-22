/*
 * A389831  Multiplicative sequence a(n) with a(p^e) = 2 + e^2 for prime p and
 *          e > 0.
 * https://oeis.org/A389831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative with a(p^e) = 2 + e^2 (oeis: A389831)
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root: floor(sqrt(n)) for non‑negative n. */
static inline idx_t integer_sqrt(idx_t n)
{
    /* Binary search – works for any idx_t implementation. */
    idx_t low  = 0;
    /* high starts as a power of two greater than sqrt(max idx_t) */
    idx_t high = (idx_t)1 << ((sizeof(idx_t) * 8) / 2 + 1);
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid != 0 && mid > n / mid)
            high = mid - 1;
        else
            low  = mid;
    }
    return low;
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* not in the range, but safe */

    if (n == 1)
        return 1;      /* a(1) = 1 */

    unsigned __int128 prod = 1;   /* use wider type to avoid premature overflow */
    uint64_t m = (uint64_t)n;

    /* factor 2 first */
    uint64_t e = 0;
    while ((m & 1ULL) == 0) {
        ++e;
        m >>= 1;
    }
    if (e) {
        prod *= (unsigned __int128)(2ULL + e * e);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= integer_sqrt(m) && m > 1; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            prod *= (unsigned __int128)(2ULL + e * e);
        }
    }

    /* if anything left > 1, it is a prime factor with exponent 1 */
    if (m > 1)
        prod *= (unsigned __int128)3ULL;   /* 2 + 1^2 */

    return (seq_t)prod;
}
