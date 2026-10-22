/*
 * A390867  The number of integers k from 1 to n such that gcd(n, k) is a
 *          bi-unitary divisor of n.
 * https://oeis.org/A390867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} a(p^e) where a(p^e) = p^e if e is odd p^e -
 *            (p-1)*p^(e/2-1) if e is even The product is taken over all prime
 *            power divisors of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq_t and idx_t are provided by the build environment. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* Undefined for 0 – return 0 safely */

    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    /* Trial division factorization – efficient enough for n up to ~1e4 */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        /* Count exponent e of prime p */
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        /* Compute p^e */
        seq_t p_pow = 1;
        for (int i = 0; i < e; ++i) p_pow *= (seq_t)p;
        /* Compute contribution a(p^e) */
        seq_t contrib;
        if (e & 1) {               /* e odd */
            contrib = p_pow;
        } else {                   /* e even */
            /* p^(e/2 - 1) */
            int exp_half_minus1 = e / 2 - 1;
            seq_t p_half_minus1 = 1;
            for (int i = 0; i < exp_half_minus1; ++i)
                p_half_minus1 *= (seq_t)p;
            /* (p-1) * p^(e/2 - 1) */
            seq_t sub = (seq_t)(p - 1) * p_half_minus1;
            contrib = p_pow - sub;
        }
        result *= contrib;
    }
    /* If remaining m > 1, it's a prime factor with exponent 1 */
    if (m > 1) {
        seq_t p_pow = (seq_t)m;         /* p^1 */
        result *= p_pow;                /* a(p) = p (since exponent is odd) */
    }
    return result;
}
