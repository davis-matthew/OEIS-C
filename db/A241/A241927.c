/*
 * A241927  Smallest k^2>=1 such that n-k^2 is semiprime p*q in Fermi-Dirac
 *          arithmetic (A176525) with additional requirement that, if n is a
 *          square, then p and q are of the same parity; or a(n)=2 if there is
 *          no such k^2.
 * https://oeis.org/A241927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1455, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9683 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search:  a(n) = smallest k^2 ≥ 1 with n‑k^2 a Fermi‑Dirac semiprime,  
   and if n is a square then the two factors have the same parity.  
   Return 2 if no such k^2 exists.  */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a 64‑bit integer is a Fermi‑Dirac prime
   (i.e., p^(2^k) with p prime, k≥0). */
static bool is_fd_prime(uint64_t x) {
    if (x <= 1) return false;
    uint64_t d = 2;
    while (d * d <= x) {
        if (x % d == 0) {
            /* d is a prime divisor of x.  Verify that it is the only one. */
            uint64_t base = d;
            unsigned exp = 0;
            while (x % d == 0) {
                x /= d;
                ++exp;
            }
            if (x != 1) return false;          /* more than one distinct prime */
            return (exp & (exp - 1)) == 0;     /* exponent is a power of two */
        }
        ++d;
    }
    /* x itself is prime; exponent 1 is a power of two. */
    return true;
}

/* Integer square root for unsigned 64‑bit numbers.  */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) {
        ++r;
    }
    return r;
}

/* Test whether m is a product of two distinct FD primes.
   The parity rule applies only when n_is_square is true. */
static bool is_fd_semiprime(uint64_t m, bool n_is_square) {
    uint64_t sqrt_m = isqrt_u64(m);
    for (uint64_t d = 2; d <= sqrt_m; ++d) {
        if (m % d != 0) continue;
        uint64_t e = m / d;
        if (d == e) continue;                     /* must be distinct */
        if (!is_fd_prime(d) || !is_fd_prime(e)) continue;
        if (n_is_square && ((d & 1ULL) != (e & 1ULL))) continue;
        return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)2;

    idx_t sqrt_n = isqrt_u64(n);
    bool n_is_square = (sqrt_n * sqrt_n == n);

    /* Try increasing k; k^2 must be < n to yield a positive difference. */
    for (idx_t k = 1; ; ++k) {
        idx_t k2 = k * k;
        if (k2 >= n) break;                /* further k will only increase k2 */
        uint64_t m = (uint64_t)n - (uint64_t)k2;
        if (m <= 1) continue;              /* need semiprime at least 4? */
        if (is_fd_semiprime(m, n_is_square))
            return (seq_t)k2;
    }

    return (seq_t)2;
}
