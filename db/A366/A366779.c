/*
 * A366779  a(n) = lambda(lambda(lambda(n))), where lambda(n) is the
 *          Carmichael lambda function (A002322).
 * https://oeis.org/A366779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = λ(λ(λ(n))) where λ is the Carmichael lambda function
 *            (Carmichael's function). This is implemented as an exact
 *            computation for any integer n≥1 using uint64_t arithmetic for
 *            the intermediate λ values. The result is cast to seq_t for
 *            return
 * Range:     u8 n <= 1438, u16 n <= 9850, u32 n <= 9850, u64 n <= 9850,
 *            u128 n <= 9850, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Carmichael lambda for a 64‑bit unsigned integer */
static uint64_t lambda_u64(uint64_t n)
{
    if (n <= 2) return 1;          /* λ(1)=1, λ(2)=1 */

    uint64_t res = 1;
    uint64_t m = n;

    /* handle factor 2 separately */
    if ((m & 1ULL) == 0) {
        int k = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++k;
        }
        uint64_t val;
        if (k == 1)           val = 1;
        else if (k == 2)      val = 2;
        else                  val = 1ULL << (k - 2);  /* 2^(k-2) */
        res = val;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int k = 0;
            while (m % p == 0) {
                m /= p;
                ++k;
            }
            /* λ(p^k) = p^(k-1)*(p-1) for odd p */
            uint64_t val = 1;
            for (int i = 1; i < k; ++i) val *= p;
            val *= (p - 1);
            uint64_t g = gcd_uint64(res, val);
            res = res / g * val;   /* lcm */
        }
    }

    /* remaining prime factor if any */
    if (m > 1) {
        uint64_t val = m - 1;      /* λ(m) = m-1 for prime m */
        uint64_t g = gcd_uint64(res, val);
        res = res / g * val;
    }

    return res;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; input n is always ≥1 in the test harness */
    uint64_t x = (uint64_t)n;
    uint64_t y = lambda_u64(x);
    uint64_t z = lambda_u64(y);
    return (seq_t)lambda_u64(z);
}
