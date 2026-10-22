/*
 * A378395  Sequence of primitive Pythagorean triples beginning with the
 *          triple (3,4,5), with each subsequent triple having as its inradius
 *          the hypotenuse of the previous triple, and with the long leg and
 *          the hypotenuse of each triple being consecutive natural numbers.
 * https://oeis.org/A378395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 13, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378395: sides of primitive Pythagorean triples
 *  (2r+1, 2r^2+2r, 2r^2+2r+1),
 * where r₁ = 1 and rₖ₊₁ = 2·rₖ² + 2·rₖ + 1.
 * Then a(3k‑2) = 2·rₖ + 1,  a(3k‑1) = 2·rₖ² + 2·rₖ,
 *     a(3k)   = 2·rₖ² + 2·rₖ + 1   (k ≥ 1). */

seq_t a(idx_t n)
{
    /* Find the index k of the triple containing a(n).  k = ceil(n/3). */
    idx_t k = (n + 2) / 3;
    seq_t r = 1;               /* r₁ */

    /* Compute rₖ by the recurrence rₖ₊₁ = 2·rₖ² + 2·rₖ + 1. */
    for (idx_t i = 1; i < k; ++i) {
        seq_t t = r * r;       /* r² */
        r = 2 * t + 2 * r + 1;
    }

    /* Now r = rₖ.  Compute the three entries of the triple. */
    seq_t t         = r * r;    /* r² */
    seq_t short_leg = 2 * r + 1;
    seq_t long_leg  = 2 * t + 2 * r; /* hypotenuse – 1 */

    /* Return the correct element based on n mod 3. */
    switch (n % 3) {
        case 1:  return short_leg;          /* a(3k‑2) */
        case 2:  return long_leg;           /* a(3k‑1) */
        default: return long_leg + 1;       /* a(3k)   */
    }
}
