/*
 * A332125  a(n) = 2*(10^(2n+1)-1)/9 + 3*10^n.
 * https://oeis.org/A332125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(10^(2n+1)-1)/9 + 3*10^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the nth term of sequence A332125.  The calculation
   is performed directly from the closed‐form definition, using
   only integer arithmetic on the seq_t type.  For seq_t types that
   are ordinary unsigned integers the intermediate values stay
   within range for all n whose a(n) fits in seq_t.  For arbitrary
   precision seq_t values the built‑in operators provide the
   necessary extended precision. */
seq_t a(idx_t n)
{
    /* Build the repunit R_{2n+1} = 111...1 (2n+1 digits). */
    seq_t repunit = 0;
    idx_t total = 2*n + 1;
    for (idx_t i = 0; i < total; ++i)
        repunit = repunit * 10 + 1;

    /* First part: 2 * R_{2n+1}. */
    seq_t first = repunit * 2;

    /* Compute 10^n. */
    seq_t pow10n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow10n = pow10n * 10;

    /* Second part: 3 * 10^n. */
    seq_t second = 3 * pow10n;

    return first + second;
}
