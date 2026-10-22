/*
 * A371381  Main diagonal of A219875.
 * https://oeis.org/A371381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + ceil(n/phi)^2 (phi = (1 + sqrt(5))/2)
 * Range:     u8 n <= 13, u16 n <= 217, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The constant NUM/DEN approximates 1/phi.
       Since phi is irrational, n/phi is never an integer;
       the chosen precision guarantees the correct ceil for all n ≤ 10⁴. */
    const uint64_t NUM = 618033988ULL;   /* floor(0.618033988... * 1e9) */
    const uint64_t DEN = 1000000000ULL;  /* scaling factor */

    /* Compute k = ceil(n/phi) = ceil(n * NUM / DEN) */
    unsigned __int128 prod = (unsigned __int128)n * NUM;
    idx_t k = (idx_t)((prod + DEN - 1) / DEN);

    /* Compute the final value a(n) = n^2 + k^2 */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t k_sq = (seq_t)k * (seq_t)k;
    return n_sq + k_sq;
}
