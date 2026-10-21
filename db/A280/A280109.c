/*
 * A280109  a(n) is the decimal value corresponding to the binary
 *          representation of the distribution of quadratic residues (value=1)
 *          and non-quadratic residues (value=0) mod n, where numbers are
 *          ordered left to right from 0 to n-1.
 * https://oeis.org/A280109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = integer whose n‑bit binary representation has 1s at *
 *            positions i (0 ≤ i < n) where i is a quadratic residue modulo n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation marks all residues by computing x^2 mod n for
 * x = 0 … n-1, then builds the number bit‑by‑bit from most significant
 * (i = 0) to least significant (i = n‑1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Allocate temporary array for residue flags */
    size_t sz = (size_t)n;
    uint8_t *qr = (uint8_t *)malloc(sz);
    if (!qr)
        return 0; /* allocation failed */

    for (size_t i = 0; i < sz; ++i)
        qr[i] = 0;

    /* Mark quadratic residues */
    for (size_t x = 0; x < sz; ++x)
    {
        size_t s = (x * x) % (size_t)n; /* x^2 mod n */
        qr[s] = 1;
    }

    /* Build the integer bit by bit (MSB = i = 0) */
    seq_t res = 0;
    for (size_t i = 0; i < sz; ++i)
        res = (res << 1) | qr[i];

    free(qr);
    return res;
}
