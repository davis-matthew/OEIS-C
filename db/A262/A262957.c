/*
 * A262957  Numerators of the n-th iteration of the alternating continued
 *          fraction formed from the positive integers, starting with (1 -
 *          ...).
 * https://oeis.org/A262957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of the n‑th iteration of the alternating continued fraction formed from the positive integers, starting with (1 - ...).
   G(j) = j + (-1)^j * 1 / G(j+1),     G(n+1) = (n+1) + (-1)^{n+1} * 1
   The final result is A(n) = 1 - 1 / G(2) = (p - q) / p, where p/q = G(2).  Hence a(n) = p - q. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, safeguard */

    idx_t n1 = n + 1;              /* base index */
    seq_t p;                       /* numerator of G(j) */
    seq_t q = 1;                   /* denominator of G(j) */

    /* Base case: G(n+1) = (n+1) + (-1)^{n+1} * 1 */
    if (n1 & 1ULL)                 /* odd: subtract 1 */
        p = (seq_t)n1 - 1;
    else                            /* even: add 1 */
        p = (seq_t)n1 + 1;

    /* Iterate j from n down to 2 */
    for (idx_t j = n; j > 1; --j) {
        seq_t old_p = p;
        seq_t old_q = q;
        if (j & 1ULL) {            /* odd j: subtract q */
            p = (seq_t)j * old_p - old_q;
        } else {                    /* even j: add q */
            p = (seq_t)j * old_p + old_q;
        }
        q = old_p;                  /* new denominator becomes old numerator */
    }

    /* A(n) = 1 - 1/(p/q) = (p - q)/p, so the numerator is p - q */
    return p - q;
}
