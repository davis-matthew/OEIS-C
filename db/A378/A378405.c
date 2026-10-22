/*
 * A378405  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(n+k,n-2*k).
 * https://oeis.org/A378405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(n+k,
 *            n-2*k)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 11, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t term = 1;           /* term corresponds to k = 0 */
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        sum += term;
        if (k == maxk) break;

        /* compute ratio to get term(k+1) from term(k) */
        unsigned __int128 num128 = (unsigned __int128)(n - k) *
                                   (unsigned __int128)(n + k + 1) *
                                   (unsigned __int128)(n - 2 * k) *
                                   (unsigned __int128)(n - 2 * k - 1);

        unsigned __int128 den128 = (unsigned __int128)(k + 1) *
                                   (unsigned __int128)(3 * k + 3) *
                                   (unsigned __int128)(3 * k + 2) *
                                   (unsigned __int128)(3 * k + 1);

        seq_t num_seq = num128;
        seq_t den_seq = den128;

        term = (term * num_seq) / den_seq;
    }
    return sum;
}
