/*
 * A397621  a(n) is the linear complexity over GF(2) of the binary expansion
 *          of n (as computed by the Berlekamp-Massey algorithm).
 * https://oeis.org/A397621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = linear complexity over GF(2) of the binary expansion of
 *            n, computed by the Berlekamp–Massey algorithm
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define MAX_BITS 70   /* enough for a 64‑bit number plus some margin */

/* Compute the linear complexity of the binary representation of n. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n=0, return 0 */

    /* Compute bit length and the bits in a most‑significant‑first array. */
    unsigned len = 0;
    idx_t tmp = n;
    while (tmp) { len++; tmp >>= 1; }

    unsigned bits[MAX_BITS];
    for (unsigned i = 0; i < len; ++i)
        bits[i] = (n >> (len - 1 - i)) & 1U;

    /* Berlekamp‑Massey variables */
    unsigned char C[MAX_BITS] = {0}, B[MAX_BITS] = {0}, T[MAX_BITS];
    unsigned L = 0, m = 1;
    C[0] = 1;
    B[0] = 1;

    for (unsigned i = 0; i < len; ++i) {
        unsigned d = bits[i];
        for (unsigned j = 1; j <= L; ++j)
            if (C[j] && bits[i - j]) d ^= 1U;

        if (d == 0) {
            ++m;
        } else {
            /* Copy C to T */
            for (unsigned j = 0; j < MAX_BITS; ++j) T[j] = C[j];

            /* C = C XOR (B << m) */
            for (unsigned j = 0; j < MAX_BITS; ++j) {
                unsigned add = (j >= m) ? B[j - m] : 0U;
                C[j] ^= add;
            }

            if (2 * L <= i) {
                L = i + 1 - L;
                for (unsigned j = 0; j < MAX_BITS; ++j) B[j] = T[j];
                m = 1;
            } else {
                ++m;
            }
        }
    }
    return (seq_t)L;
}
