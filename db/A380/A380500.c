/*
 * A380500  Table T(n,k) = phi(phi(prime(n)^k)), n >= 1, k >= 0, read by
 *          upwards antidiagonals, where phi = A000010.
 * https://oeis.org/A380500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(i+1,j) where (i,j) are the 0‑based antidiagonal
 *            coordinates and T(n,k) = φ(φ(prime(n)^k)). The coordinates are
 *            obtained by iterating over the diagonals s = i + j, with each
 *            diagonal listed from the bottom‑left element (i = s, j = 0)
 *            upwards to the top‑right (i = 0, j = s)
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 115, u64 n <= 269,
 *            u128 n <= 691, big any n
 * Helpers:   big_pow, calc_phi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/calc_phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find diagonal s such that (s+1)(s+2)/2 ≥ n */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 < n) {
        ++s;
    }

    /* Number of entries before diagonal s */
    idx_t prev = s * (s + 1) / 2;

    /* Offset within the diagonal (0‑based) */
    idx_t offset = n - prev - 1;

    /* Convert to 0‑based row (i) and exponent (j) */
    idx_t i = s - offset;   /* row index */
    idx_t j = offset;       /* k exponent */

    /* Get the (i+1)-th prime */
    uint32_t p32 = get_prime(i + 1);
    seq_t p = (seq_t)p32;

    /* k = 0 gives 1 */
    if (j == 0) {
        return (seq_t)1;
    }

    /* φ(p−1) needed for k ≥ 1 */
    seq_t phi_val = calc_phi((seq_t)(p - (seq_t)1));

    /* k = 1 */
    if (j == 1) {
        return phi_val;
    }

    /* k ≥ 2 : compute (p−1) * φ(p−1) and multiply by p^(k−2) */
    seq_t p_minus = p - (seq_t)1;
    seq_t t2 = p_minus * phi_val;
    seq_t pow_factor = big_pow(p, (seq_t)(j - (seq_t)2));

    return pow_factor * t2;
}
