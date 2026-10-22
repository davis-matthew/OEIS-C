/*
 * A385606  Diagonal of the rational function 1/(1 - (v^3 + w^3 + x^3 + y^3 +
 *          z^3 + v*w*x*y*z)).
 * https://oeis.org/A385606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} (n+2k)!/(k!^5 * (n-3k)!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t next_term(idx_t n, idx_t k, seq_t term) {
    /* term_k → term_{k+1} */
    seq_t numerators[5];
    seq_t denominators[5];
    numerators[0] = (seq_t)(n + 2*k + 2);
    numerators[1] = (seq_t)(n + 2*k + 1);
    numerators[2] = (seq_t)(n - 3*k);
    numerators[3] = (seq_t)(n - 3*k - 1);
    numerators[4] = (seq_t)(n - 3*k - 2);
    for (uint_fast8_t i = 0; i < 5; ++i)
        denominators[i] = (seq_t)(k + 1);

    /* cancel common factors between term and denominators */
    for (uint_fast8_t i = 0; i < 5; ++i) {
        if (denominators[i] <= 1) continue;
        seq_t g = gcd(term, denominators[i]);
        if (g > 1) {
            term /= g;
            denominators[i] /= g;
        }
    }

    /* process each numerator factor */
    for (uint_fast8_t i = 0; i < 5; ++i) {
        seq_t f = numerators[i];
        /* cancel any denominators with this numerator */
        for (uint_fast8_t j = 0; j < 5; ++j) {
            if (denominators[j] <= 1) continue;
            seq_t g = gcd(f, denominators[j]);
            if (g > 1) {
                f /= g;
                denominators[j] /= g;
            }
        }
        /* multiply term by the reduced numerator */
        term = term * f;
        /* after each multiplication try to reduce against remaining denominators */
        for (uint_fast8_t j = 0; j < 5; ++j) {
            if (denominators[j] <= 1) continue;
            if (term % denominators[j] == 0) {
                term /= denominators[j];
                denominators[j] = 1;
            }
        }
    }

    /* divide any remaining denominators */
    for (uint_fast8_t i = 0; i < 5; ++i)
        if (denominators[i] > 1)
            term /= denominators[i];

    return term;
}

seq_t a(idx_t n) {
    idx_t kmax = n / 3;
    seq_t sum = 0;
    seq_t term = 1;          /* term_0 = 1 */

    for (idx_t k = 0; ; ++k) {
        sum += term;
        if (k == kmax)
            break;
        term = next_term(n, k, term);
    }
    return sum;
}
