/*
 * A380790  Length of the n-th Golomb ruler constructed by the Paul Erdős and
 *          Pál Turán formula.
 * https://oeis.org/A380790
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p-1) * p^2 + 1 + Σ_{k=2..p-1} (k^2 mod p) , * where p =
 *            the nth prime (OEIS offset 2)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 257, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined for n < 2, return 0 */

    /* obtain the nth prime */
    uint32_t p_u = get_prime(n);
    seq_t p = (seq_t)p_u;

    /* base term */
    seq_t base = (p - 1) * p * p + 1;

    /* sum over k = 2..p-1 of k^2 mod p */
    seq_t s = 0;
    for (idx_t k = 2; k < p_u; ++k) {
        seq_t kk = (seq_t)k;
        s += (kk * kk) % p;
    }

    return base + s;
}
