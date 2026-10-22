/*
 * A351606  a(n) = n^8 * Sum_{d^2|n} 1 / d^8.
 * https://oeis.org/A351606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^8 * Sum_{d^2|n} 1/d^8
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative: a(p^e) = p^8 * (p^(8e) - p^(8⌊(e-1)/2⌋)) / (p^8 - 1) */

#include <stdint.h>
#include <stddef.h>

/* Integer square‑root without overflow */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {          /* handle zero specially */
            ans = 0;
            lo = 1;
            continue;
        }
        if (mid <= n / mid) {    /* mid*mid <= n without overflow */
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* Fast exponentiation: base^exp */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1U) result = result * b;
        b = b * b;
        exp >>= 1U;
    }
    return result;
}

/* Compute the contribution of a prime power p^e */
static inline seq_t factor_value(idx_t p, idx_t e)
{
    /* p^8 */
    seq_t p8 = pow_seq((seq_t)p, (idx_t)8);
    /* p^(8e) and p^(8⌊(e-1)/2⌋) */
    seq_t num   = pow_seq(p8, e) - pow_seq(p8, (e - 1) / 2);
    seq_t den   = p8 - 1;
    seq_t div   = num / den;           /* exact division */
    return p8 * div;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n=0, but safe */

    seq_t res = 1;
    idx_t m   = n;

    /* factor 2 separately for speed */
    idx_t e2 = 0;
    while ((m & 1ULL) == 0) {
        ++e2;
        m >>= 1;
    }
    if (e2) res = res * factor_value(2, e2);

    /* trial division for odd primes up to sqrt(m) */
    idx_t limit = isqrt_u64(m);
    for (idx_t p = 3; p <= limit; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            res = res * factor_value(p, e);
            limit = isqrt_u64(m);      /* update limit if m shrinks */
        }
    }

    /* remaining prime factor */
    if (m > 1) res = res * factor_value(m, 1);

    return res;
}
