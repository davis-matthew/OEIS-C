/*
 * A360112  Number of solutions to m^(1 + 2^v(n-1)) == -m (mod n), where v(n)
 *          = A007814(n) is the 2-adic valuation of n, and 0 <= m < n.
 * https://oeis.org/A360112
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   Number of solutions to m^(1 + 2^v(n-1)) ≡ -m (mod n) for 0 ≤ m <
 *            n
 * Range:     u8 n <= 9803, u16 n <= 9803, u32 n <= 9803, u64 n <= 9803,
 *            u128 n <= 9803, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count solutions for given n (n ≥ 2).  Uses brute‑force modular exponentiation. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;
    /* 2-adic valuation of (n-1).  For n ≥ 2, n-1 > 0. */
    uint64_t v = __builtin_ctzll((uint64_t)(n - 1));
    uint64_t exp = 1ULL + (1ULL << v);        /* exponent 1 + 2^v */

    uint64_t count = 0;
    const uint64_t mod = (uint64_t)n;

    for (uint64_t m = 0; m < mod; ++m)
    {
        /* modular exponentiation: m^exp mod n */
        uint64_t result = 1;
        uint64_t base   = m % mod;
        uint64_t e      = exp;

        while (e)
        {
            if (e & 1ULL)
                result = (result * base) % mod;
            base = (base * base) % mod;
            e >>= 1ULL;
        }

        /* check if result == -m (mod n) */
        if (result == (mod - m) % mod)
            ++count;
    }

    return (seq_t)count;
}
