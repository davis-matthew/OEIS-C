/*
 * A257302  Number of 4th power nonresidues modulo n.
 * https://oeis.org/A257302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - number of distinct fourth power residues modulo n
 * Range:     u8 n <= 270, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but safe */

    /* Allocate a flag array of size n to mark residues seen. */
    uint8_t *seen = (uint8_t*)calloc((size_t)n, sizeof(uint8_t));
    if (!seen) return 0;   /* fallback: allocation failed */

    seq_t distinct = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t m = (seq_t)i;
        seq_t r = m;
        r = (r * m) % (seq_t)n;     /* m^2 mod n */
        r = (r * m) % (seq_t)n;     /* m^3 mod n */
        r = (r * m) % (seq_t)n;     /* m^4 mod n */

        /* r < n, so it is safe to cast to an array index. */
        if (!seen[(size_t)r]) {
            seen[(size_t)r] = 1;
            ++distinct;
        }
    }
    free(seen);

    return ((seq_t)n) - distinct;
}
