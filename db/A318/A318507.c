/*
 * A318507  a(n) = A032742(n) XOR A001065(n)-A032742(n), where XOR is
 *          bitwise-or (A003987) and A001065 = sum of proper divisors and
 *          A032742 = the largest proper divisor of n.
 * https://oeis.org/A318507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A032742(n) XOR (A001065(n)-A032742(n))
 * Range:     u8 n <= 179, u16 n <= 9705, u32 n <= 9705, u64 n <= 9705,
 *            u128 n <= 9705, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Smallest prime factor: trial division, n <= 10000 */
static uint64_t _smallest_prime_factor(uint64_t n)
{
    if (n < 2) return n;
    for (uint64_t p = 2; p * p <= n; ++p)
        if (n % p == 0) return p;
    return n;
}

/* Sum of all divisors σ(n) computed by factorisation */
static seq_t _sigma_of(uint64_t n)
{
    uint64_t temp = n;
    unsigned __int128 sigma = 1;
    while (temp > 1) {
        uint64_t p = 0;
        for (uint64_t d = 2; d * d <= temp; ++d) {
            if (temp % d == 0) { p = d; break; }
        }
        if (p == 0) p = temp;          /* remaining prime */
        uint64_t pow = 1;
        while (temp % p == 0) {
            temp /= p;
            pow *= p;
        }
        unsigned __int128 num   = (unsigned __int128)pow * p - 1;
        unsigned __int128 factor = num / (p - 1);
        sigma *= factor;
    }
    return (seq_t)sigma;
}

/* The requested sequence */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    uint64_t N = (uint64_t)n;

    /* largest proper divisor = N / smallest prime factor */
    uint64_t lpf = _smallest_prime_factor(N);
    uint64_t largest = N / lpf;

    /* sum of proper divisors = σ(N) – N */
    seq_t sigma  = _sigma_of(N);
    seq_t sumproper = sigma - (seq_t)N;

    /* a(n) = largest XOR (sumproper – largest) */
    seq_t diff  = sumproper - (seq_t)largest;
    return (seq_t)largest ^ diff;
}
