/*
 * A243822  Number of k < n such that rad(k) | n but k does not divide n,
 *          where rad = A007947.
 * https://oeis.org/A243822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k < n | rad(k) | n and k ∤ n }
 * Range:     u8 n <= 843, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 1; k < n; ++k) {
        if ((n % k) == 0)      /* k divides n – skip */
            continue;
        seq_t rad_k_seq = rad(k);          /* rad(k) is a seq_t number */
        idx_t rad_k = (idx_t)rad_k_seq;    /* safe: rad(k) ≤ k < n */
        if ((n % rad_k) == 0)              /* rad(k) divides n – count */
            ++cnt;
    }
    return cnt;
}
