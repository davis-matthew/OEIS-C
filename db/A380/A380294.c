/*
 * A380294  The Golomb-Rice encoding of n, with M = A070939(A070939(n)).
 * https://oeis.org/A380294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (2^q - 1)*2^A070939(m) + r, where m =
 *            2^A070939(A070939(n)), q = floor(n/m), r = n mod m
 * Range:     u8 n <= 39, u16 n <= 103, u32 n <= 447, u64 n <= 959,
 *            u128 n <= 1983, big any n
 * Helpers:   bitlen_seq_t, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* defined trivially for n = 0 */

    /* k1 = A070939(n) = bitlen(n) */
    idx_t k1 = bitlen_seq_t((seq_t)n);

    /* k2 = A070939(k1) */
    idx_t k2 = bitlen_seq_t((seq_t)k1);

    /* m = 2^k2 as an idx_t (small enough to fit in 64 bits) */
    idx_t m_idx = 1ULL << k2;

    /* q and r for the division n / m */
    idx_t q = n / m_idx;
    idx_t r = n % m_idx;

    /* k3 = A070939(m) = bitlen(m) */
    idx_t k3 = bitlen_seq_t((seq_t)m_idx);

    /* Compute (2^q - 1)*2^k3 + r using seq_t arithmetic */
    seq_t pow2q  = pow2(q);      /* 2^q   */
    seq_t pow2k3 = pow2(k3);     /* 2^k3  */

    seq_t result = (pow2q - 1) * pow2k3 + (seq_t)r;
    return result;
}
