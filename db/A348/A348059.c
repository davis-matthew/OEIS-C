/*
 * A348059  a(n) = Min {k > n : A235137(k) == -n (mod k)}, or -1 if no such
 *          minimum exists.
 * https://oeis.org/A348059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min {k > n : A235137(k) == -n (mod k)}, found by
 *            searching k = n+1, n+2, ... For q = p^e exactly dividing k,
 *            every unit u satisfies u^phi(k) == 1 (mod q), and every multiple
 *            of p satisfies j^phi(k) == 0 (mod q), so A235137(k) ==
 *            (k/q)*phi(q) (mod q). The test is therefore: q | (k/q)*phi(q)...
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A348059_ok(uint64_t k, uint64_t n)
{
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t q = 1;
            while (m % p == 0) { m /= p; q *= p; }
            uint64_t phiq = q / p * (p - 1);
            unsigned __int128 v = (unsigned __int128)((k / q) % q) * (phiq % q) + (n % q);
            if (v % q != 0) return false;
        }
    }
    if (m > 1) {
        uint64_t q = m;
        uint64_t phiq = q - 1;
        unsigned __int128 v = (unsigned __int128)((k / q) % q) * (phiq % q) + (n % q);
        if (v % q != 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t nn = (uint64_t)n;
    for (uint64_t k = nn + 1; ; ++k) {
        if (A348059_ok(k, nn)) return (seq_t)k;
    }
}
