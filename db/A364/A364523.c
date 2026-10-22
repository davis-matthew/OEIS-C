/*
 * A364523  G.f. satisfies A(x) = 1 + x*A(x) + x^6*A(x)^6.
 * https://oeis.org/A364523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/6⌋} binomial(n,6*k)*binomial(6*k,k)/(5*k+1)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 54, u128 n <= 101,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n / 6;
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t s6k = 6 * k;
        seq_t bin1 = binomial(n, s6k);
        seq_t bin2 = binomial(s6k, k);
        seq_t term;
        /* Use 128‑bit intermediate when seq_t is 64‑bit */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 prod = (unsigned __int128)bin1 * (unsigned __int128)bin2;
            term = (seq_t)(prod / (5 * k + 1));
        } else {
            term = (bin1 * bin2) / (seq_t)(5 * k + 1);
        }
        sum += term;
    }
    return sum;
}
