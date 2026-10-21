/*
 * A245579  Number of odd divisors of n multiplied by n.
 * https://oeis.org/A245579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * A001227(n) (number of odd divisors of n times n)
 * Range:     u8 n <= 44, u16 n <= 3464, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: integer square root for idx_t (assumed to be unsigned 64-bit). */
static inline idx_t isqrt_u64(idx_t n) {
    if (n < 2) return n;
    idx_t r = 0;
    idx_t bit = (idx_t)1U << 62;  /* highest power of four <= n */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Count the number of odd divisors of m. */
static seq_t odd_divisor_count(idx_t m)
{
    seq_t count = 1;
    idx_t temp = m;
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            count *= (seq_t)(e + 1);
        }
    }
    if (temp > 1) {
        /* Remaining prime factor > 1 */
        count *= 2;
    }
    return count;
}

/* Sequence element a(n) for OEIS A245579. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Remove all factors of 2 to obtain the odd part. */
    idx_t odd_part = n;
    while ((odd_part & 1ULL) == 0) {
        odd_part >>= 1;
    }

    seq_t odd_div_cnt = odd_divisor_count(odd_part);
    return (seq_t)n * odd_div_cnt;
}
