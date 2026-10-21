/*
 * A255808  Numbers with no zeros in base-9 representation.
 * https://oeis.org/A255808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 203, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th positive integer with no zeros in base-9 representation
   Computation via brute-force search for small n. */
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t val = (seq_t)1;        // first term a(1)=1
    idx_t count = 1;             // number of valid terms found
    while (count < n) {
        val += (seq_t)1;         // next integer to test
        seq_t t = val;
        bool ok = true;
        while (t != (seq_t)0) {
            if ((t % (seq_t)9) == (seq_t)0) {  // base-9 digit zero found
                ok = false;
                break;
            }
            t /= (seq_t)9;
        }
        if (ok) ++count;
    }
    return val;
}
