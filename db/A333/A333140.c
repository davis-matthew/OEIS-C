/*
 * A333140  a(n) is the least multiple of n of the form Sum_{k >= 0} e_k * b^k
 *          where b > 2 and Sum_{k >= 0} e_k * 2^k is the binary
 *          representation of n.
 * https://oeis.org/A333140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the binary representation of n in the smallest
 *            base b>2 such that the resulting number is a multiple of n. The
 *            value returned is that interpreted number
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 66, u64 n <= 261,
 *            u128 n <= 3186, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety, though n>=1 in the sequence */

    /* Find the index of the most significant bit of n. */
    idx_t msb = 0;
    idx_t t = n;
    while (t > 1) {
        t >>= 1;
        ++msb;
    }

    /* Search for the minimal base b > 2 such that the polynomial
       P(b) = Σ e_k * b^k is divisible by n.               */
    idx_t b = 3;                /* start from the smallest allowed base */
    for (;;) {
        idx_t rem = 0;
        /* Horner evaluation of the polynomial modulo n. */
        for (idx_t i = msb; ; --i) {
            /* Use 128‑bit arithmetic to avoid overflow. */
            __int128 prod = (__int128)rem * b + ((n >> i) & 1);
            rem = (idx_t)(prod % n);
            if (i == 0) break;
        }
        if (rem == 0) break;   /* found the minimal base */
        ++b;
    }

    /* Compute the exact value P(b) in the result type seq_t. */
    seq_t value = 0;
    seq_t power = 1;          /* b^0 initially */
    seq_t base = (seq_t)b;    /* cast once for efficiency */

    for (idx_t i = 0; i <= msb; ++i) {
        if ((n >> i) & 1)
            value += power;
        power *= base;
    }

    return value;
}
