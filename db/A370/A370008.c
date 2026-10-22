/*
 * A370008  a(n) is the greatest prime less than 3*prime(n).
 * https://oeis.org/A370008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = greatest prime less than 3*prime(n) (search for the previous prime) */
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n) {
    /* retrieve the n-th prime; indexing is 1‑based */
    uint32_t pn = get_prime(n);

    /* threshold 3*prime(n) */
    seq_t limit = (seq_t)pn * 3;

    /* search downward for the greatest prime < limit */
    seq_t candidate = limit - 1;
    while (!isPrimeGeneric(candidate)) {
        candidate -= 1;
    }
    return candidate;
}
