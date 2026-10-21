/*
 * A246200  Self-inverse permutation of natural numbers: a(n) = A057889(3*n) /
 *          3.
 * https://oeis.org/A246200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A057889(3*n) / 3 * A057889: balanced bit-reversal of its
 *            argument. * For a number x: * 1. Let t = number of trailing
 *            zeros of x. * 2. Let len = bit-length of x. * 3. Reverse the
 *            len-bit binary representation of x, obtaining rev. * 4. Return
 *            rev << t. * Then a(n) = (A057889(3*n)) / 3. * The...
 * Range:     u8 n <= 171, u16 n <= 9532, u32 n <= 9532, u64 n <= 9532,
 *            u128 n <= 9532, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Step 1: compute 3 * n using seq_t arithmetic */
    seq_t m = (seq_t)n * 3;
    if (m == 0) {           /* a(0) = 0 */
        return 0;
    }

    /* Step 2: compute bit-length of m */
    idx_t len = 0;
    seq_t tmp = m;
    while (tmp) {
        len++;
        tmp >>= 1;
    }

    /* Step 3: reverse the len bits of m */
    seq_t rev = 0;
    tmp = m;
    for (idx_t i = 0; i < len; i++) {
        rev = (rev << 1) | ((tmp >> i) & 1);
    }

    /* Step 4: count trailing zeros of m */
    idx_t t = 0;
    tmp = m;
    while ((tmp & 1) == 0) {
        t++;
        tmp >>= 1;
    }

    /* Step 5: shift left by t and divide by 3 */
    rev <<= t;
    return rev / 3;
}
