/*
 * A383075  Smallest number m such that m*(m + 1)*(2*m + 1)/6 is divisible by
 *          n.
 * https://oeis.org/A383075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m ≥ 1 such that m*(m + 1)*(2*m + 1)/6 is
 *            divisible by n * Equivalently, m*(m + 1)*(2*m + 1) ≡ 0 (mod 6*n)
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The problem reduces to finding the least m such that
     *   m*(m+1)*(2*m+1)  is a multiple of 6*n.
     */
    seq_t mod = (seq_t)6 * (seq_t)n;          /* 6*n  (the divisor) */
    idx_t  m   = 1;                           /* candidate m */

    /* Avoid overflow by working modulo 'mod' at each step.
     * Since mod fits in seq_t (n ≤ 10⁴ ⇒ mod ≤ 6·10⁴), it is safe. */
    seq_t m_mod = (seq_t)1 % mod;              /* current m  modulo mod */

    while (1) {
        /* Compute (m*(m+1)*(2*m+1)) modulo mod */
        seq_t prod = (m_mod * (m_mod + 1)) % mod;
        prod = (prod * ((seq_t)2 * m_mod + (seq_t)1)) % mod;

        if (prod == 0)                     /* divisibility achieved */
            return (seq_t)m;

        /* Advance to next m */
        ++m;
        m_mod = (m_mod + (seq_t)1) % mod;   /* (m+1) % mod */
    }
}
