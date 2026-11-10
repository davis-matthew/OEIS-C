/*
 * A284261  a(n) = A284258(n) - A284258(A003961(n)).
 * https://oeis.org/A284261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A284258(n) - A284258(A003961(n)), where A284258(n) =
 *            number of distinct primes dividing n that exceed (smallest prime
 *            factor of n)^2, and A003961 shifts each prime to the next prime
 * Range:     u8 n <= 10001, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 136 terms through n = 9539 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A284261_isp(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

static uint64_t A284261_nextp(uint64_t p)
{
    uint64_t q = p + 1;
    while (!A284261_isp(q)) q++;
    return q;
}

/* A284258: distinct primes of n greater than spf(n)^2 */
static uint64_t A284261_cnt(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t spf = 0, res = 0;
    uint64_t m = n;
    uint64_t ps[64];
    int k = 0;
    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            ps[k++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) ps[k++] = m;
    spf = ps[0];
    unsigned __int128 b = (unsigned __int128)spf * spf;
    for (int i = 0; i < k; i++)
        if ((unsigned __int128)ps[i] > b) res++;
    return res;
}

/* A003961: completely multiplicative, prime(k) -> prime(k+1) */
static uint64_t A284261_shift(uint64_t n)
{
    uint64_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            uint64_t q = A284261_nextp(p);
            while (m % p == 0) {
                m /= p;
                result *= q;
            }
        }
    }
    if (m > 1) result *= A284261_nextp(m);
    return result;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    uint64_t c1 = A284261_cnt(x);
    uint64_t c2 = A284261_cnt(A284261_shift(x));
    return (seq_t)c1 - (seq_t)c2;
}
