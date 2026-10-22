/*
 * A308501  Partial products of odious numbers.
 * https://oeis.org/A308501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} A000069(k) (product of the first n odious
 *            numbers, a(0)=1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 1;

    seq_t prod = 1;
    idx_t odious_found = 0;
    uint64_t num = 1;

    /* Scan natural numbers, pick those with odd popcount (odious). */
    while (odious_found < n) {
        if (__builtin_parityll(num)) {  /* true iff num has an odd number of 1-bits */
            prod *= (seq_t)num;
            ++odious_found;
        }
        ++num;
    }
    return prod;
}
