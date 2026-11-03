/*
 * A345068  a(n) = Sum_{d|n, d>1} d^floor(1/omega(d)).
 * https://oeis.org/A345068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n, d>1} d^⌊1/ω(d)⌋ , where ω(d) is the number of
 *            distinct prime divisors of d
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 8876 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0 */

    seq_t sum = 0;
    idx_t nn = n;

    for (idx_t i = 1; i <= nn / i; ++i) {
        if (nn % i == 0) {
            /* divisor i */
            idx_t d1 = i;
            if (d1 > 1) {
                int omega = count_distinct_primes((uint64_t)d1);
                if (omega == 1)
                    sum += (seq_t)d1;   /* prime power: add d */
                else
                    sum += (seq_t)1;    /* composite with ≥2 primes: add 1 */
            }

            /* paired divisor nn / i, avoid double‑counting when i*i==nn */
            idx_t d2 = nn / i;
            if (d2 != d1 && d2 > 1) {
                int omega = count_distinct_primes((uint64_t)d2);
                if (omega == 1)
                    sum += (seq_t)d2;
                else
                    sum += (seq_t)1;
            }
        }
    }

    return sum;
}
