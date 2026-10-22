/*
 * A333783  a(n) = sigma(n) - A332993(n).
 * https://oeis.org/A333783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sigma(n) - sum of the chain of largest proper divisors of
 *            n (i.e., largest divisor obtained by repeatedly dividing by the
 *            smallest prime factor, starting from n and ending at 1)
 * Range:     u8 n <= 239, u16 n <= 9166, u32 n <= 9166, u64 n <= 9166,
 *            u128 n <= 9166, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t _smallest_prime_factor(uint64_t n)
{
    if (n <= 1) return n;
    if (n % 2 == 0) return 2;
    for (uint64_t i = 3; i * i <= n; i += 2)
        if (n % i == 0) return i;
    return n; /* n is prime */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of bounds, not used */
    uint64_t x = (uint64_t)n;
    seq_t sigma = 1;

    /* Compute sigma(n) by prime factorization */
    while (x > 1) {
        uint64_t p = _smallest_prime_factor(x);
        uint64_t e = 0;
        while (x % p == 0) {
            x /= p;
            e++;
        }
        /* sum_p = 1 + p + p^2 + ... + p^e */
        seq_t sum_p = 1;
        seq_t term   = 1;
        for (uint64_t i = 1; i <= e; ++i) {
            term   *= (seq_t)p;
            sum_p  += term;
        }
        sigma *= sum_p;
    }

    /* Compute chain sum of largest proper divisors */
    seq_t chain_sum = 0;
    uint64_t cur = (uint64_t)n;
    while (1) {
        chain_sum += (seq_t)cur;
        if (cur == 1) break;
        uint64_t spf = _smallest_prime_factor(cur);
        cur /= spf;
    }

    return sigma - chain_sum;
}
