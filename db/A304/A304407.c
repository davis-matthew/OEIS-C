/*
 * A304407  If n = Product (p_j^k_j) then a(n) = Product ((p_j - 1)*k_j).
 * https://oeis.org/A304407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^k∥n} (p-1)·k
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 91 terms through n = 973 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t first_prime_factor(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return d;
    }
    return n; /* n is prime */
}

/* Compute a(n) for A304407 */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    uint64_t N = (uint64_t)n;              /* local copy for factorisation */
    seq_t result = (seq_t)1;

    while (N > 1) {
        uint64_t p = first_prime_factor(N);
        uint64_t k = 0;
        while (N % p == 0) {
            N /= p;
            ++k;
        }
        result *= (seq_t)(p - 1) * (seq_t)k;
    }

    return result;
}
