/*
 * A392879  Let p = A129252(n), if p = 1, then a(n) = n, otherwise a(n) = n *
 *          p^((e mod p)-e) * q^floor(e/p), where e is p-adic valuation of n
 *          and q is the least prime number greater than p.
 * https://oeis.org/A392879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 9869, u32 n <= 9869, u64 n <= 9869,
 *            u128 n <= 9869, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A392879: a(n) = n, if no prime p divides n with exponent e >= p;
   otherwise a(n) = n / p^(e - e mod p) * q^(floor(e / p)),
   where q = the least prime > p, p = A129252(n). */

#include <stdint.h>
#include "../utils/next_prime.h"

/* Integer power for seq_t base and unsigned 64‑bit exponent. */
static inline seq_t pow_seq_int(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)            /* A129252(1) = 1, so a(1) = 1 */
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;      /* local copy for factoring */
    uint64_t m   = nn;
    uint64_t min_p = 0;             /* zero -> no overflow prime found */
    uint64_t min_e = 0;

    /* Trial division to factor n */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e >= p) {
                if (min_p == 0 || p < min_p) {
                    min_p = p;
                    min_e = e;
                }
            }
        }
    }
    /* Remaining prime factor (if any) */
    if (m > 1) {
        uint64_t p = m;
        uint64_t e = 1;
        if (e >= p) {
            if (min_p == 0 || p < min_p) {
                min_p = p;
                min_e = e;
            }
        }
    }

    /* No overflow prime → a(n) = n */
    if (min_p == 0)
        return (seq_t)nn;

    uint64_t p = min_p;
    uint64_t e = min_e;
    uint64_t e_div = e / p;           /* floor(e/p) */
    uint64_t e_mod = e % p;           /* e mod p */

    /* least prime greater than p */
    uint64_t q = next_prime(p);

    /* Compute a(n) = n / p^(e - e_mod) * q^e_div */
    seq_t result = (seq_t)nn;
    seq_t p_pow   = pow_seq_int((seq_t)p, e - e_mod);
    result /= p_pow;
    seq_t q_pow   = pow_seq_int((seq_t)q, e_div);
    result *= q_pow;

    return result;
}
