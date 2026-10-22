/*
 * A358680  a(n) = 1 if the arithmetic derivative of n is even, 0 otherwise.
 * https://oeis.org/A358680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the arithmetic derivative of n is even, 0 otherwise
 * Range:     u8 n <= 9989, u16 n <= 9989, u32 n <= 9989, u64 n <= 9989,
 *            u128 n <= 9989, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;          /* convert index to 64‑bit value */
    /* For n = 0 or 1 the arithmetic derivative is 0 (even). */
    if (nn <= 1u)
        return (seq_t)1;

    uint64_t tmp = nn;
    uint64_t parity = 0;                /* 0 = even derivative, 1 = odd */

    /* Trial division to factor n and accumulate parity of the sum
       n * Σ (e_i / p_i).  Only odd exponents contribute. */
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (exp & 1u)               /* odd exponent */
                parity ^= (nn / p) & 1; /* add (n/p) modulo 2 */
        }
    }
    /* If a prime factor > sqrt(nn) remains, its exponent is 1 (odd). */
    if (tmp > 1u)
        parity ^= (nn / tmp) & 1;

    /* parity == 0 → even derivative → return 1, else 0. */
    return (seq_t)(parity == 0 ? 1 : 0);
}
