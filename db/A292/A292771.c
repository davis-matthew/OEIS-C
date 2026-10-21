/*
 * A292771  If sigma(n)-phi(n) is even then (sigma(n)-phi(n))/2 otherwise -1.
 * https://oeis.org/A292771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (sigma(n)-phi(n))/2 if sigma(n)-phi(n) even, otherwise -1
 * Range:     i8 n <= 59, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t phi_val = phi(n);
    seq_t diff = sigma - phi_val;
    if ((diff & 1) == 0) {
        return diff / 2;
    } else {
        return (seq_t)-1;
    }
}
