/*
 * A351353  Numbers k such that k^2 is a centered 40-gonal number.
 * https://oeis.org/A351353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(3*n‑2) for odd n, Lucas(3*n‑1) for even n (Lucas
 *            numbers are defined by L₀ = 2, L₁ = 1, L_{k} = L_{k‑1} +
 *            L_{k‑2})
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   lucas_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lucas_k.h"

/* External sequence function.  n ≥ 1.  n == 0 is undefined but returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Index of the Lucas number: 3n‑2 for odd n, 3n‑1 for even n. */
    idx_t idx = (n & 1U) ? (3 * n - 2) : (3 * n - 1);

    return lucas_k(idx);
}
