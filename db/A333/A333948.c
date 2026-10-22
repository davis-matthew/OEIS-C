/*
 * A333948  Number of integer-sided triangles with perimeter n whose side
 *          lengths have the same number of divisors.
 * https://oeis.org/A333948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer‑sided triangles with perimeter n whose
 *            side lengths have the same number of divisors
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 3) return 0;

    /* Precompute divisor counts up to n */
    idx_t nn = n;
    uint32_t *divcnt = (uint32_t *)malloc((nn + 1) * sizeof(uint32_t));
    if (!divcnt) return 0;
    for (idx_t i = 0; i <= nn; i++) divcnt[i] = 0;
    for (idx_t i = 1; i <= nn; i++) {
        for (idx_t j = i; j <= nn; j += i) divcnt[j] += 1U;
    }

    seq_t result = 0;
    for (idx_t a = 1; a <= nn / 3; a++) {
        uint32_t dA = divcnt[a];
        for (idx_t b = a; b <= (nn - a) / 2; b++) {
            uint32_t dB = divcnt[b];
            idx_t c = nn - a - b;
            if (c < b) continue;                  /* maintain a <= b <= c */
            if (a + b <= c) continue;             /* triangle inequality */
            if (dA != dB) continue;               /* a and b must match */
            if (divcnt[c] != dA) continue;        /* all three equal */
            result += 1;
        }
    }

    free(divcnt);
    return result;
}
