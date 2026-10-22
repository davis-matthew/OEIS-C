/*
 * A380161  a(n) is the value of the Euler totient function when applied to
 *          the powerfree part of n.
 * https://oeis.org/A380161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A380161 – a(n) = ∏_{p|n, v_p(n)=1} (p-1)   */
/*  a(n) is the Euler totient of the “power‑free” part of n,  */
/*  i.e. of the product of those primes that divide n to the first power.   */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1u) return 1;          /* a(1)=1,  a(0) is not used */

    uint64_t m = (uint64_t)n;       /* work on a 64‑bit copy of n   */
    seq_t prod = 1;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (exp == 1)               /* include only once‑exponent primes */
                prod = prod * (seq_t)(p - 1);
        }
    }

    /* remaining factor, if any, is a prime with exponent 1 */
    if (m > 1)
        prod = prod * (seq_t)(m - 1);

    return prod;
}
