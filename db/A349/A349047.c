/*
 * A349047  G.f. A(x) satisfies: A(x) = 1 / (1 - x + x^3 * A(x)).
 * https://oeis.org/A349047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(n-k, 2*k) *
 *            Catalan(k)
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 37, i64 n <= 70, i128 n <= 136,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A349047 – signed sequence defined by a(0)=a(1)=a(2)=1 and for n≥3: */
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n) {
    /* compute directly from the binomial/Catalan sum */
    idx_t maxk = n / 3;
    seq_t result = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c = binomial(n - k, 2 * k);
        c *= catalan(k);
        if (k & 1)
            result -= c;
        else
            result += c;
    }
    return result;
}
