/*
 * A318517  a(n) = A032742(n) XOR n-A032742(n), where XOR is bitwise-xor
 *          (A003987) and A032742 = the largest proper divisor of n.
 * https://oeis.org/A318517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest proper divisor of n XOR (n - largest proper
 *            divisor of n)
 * Range:     u8 n <= 256, u16 n <= 9443, u32 n <= 9443, u64 n <= 9443,
 *            u128 n <= 9443, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n == 1 explicitly: 1 has no proper divisor except 0 in this context. */
    if (n == 1) {
        return (seq_t)1; /* 0 ^ 1 = 1 */
    }
    if (n == 0) {
        return (seq_t)0; /* Out of range, just return 0. */
    }

    /* Find the smallest prime factor of n (trial division). */
    idx_t spf = n;
    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            spf = i;
            break;
        }
    }

    /* Largest proper divisor is n / spf. */
    seq_t d = (seq_t)n / (seq_t)spf;

    /* Compute a(n) = d XOR (n - d). */
    return d ^ ((seq_t)n - d);
}
