/*
 * A341833  Dirichlet g.f.: 1 / zeta(s)^7.
 * https://oeis.org/A341833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = product_{p^e||n} (-1)^e * binomial(7,e) , a(1)=1
 * Range:     i8 n <= 11, i16 n <= 2519, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;

    /* binomial(7,k) for k = 0..7 */
    static const seq_t bin[8] = {1, 7, 21, 35, 35, 21, 7, 1};

    seq_t result = 1;
    idx_t tmp = n;

    /* factor 2 */
    int e = 0;
    while ((tmp & 1ULL) == 0 && tmp != 0) {
        e++;
        tmp >>= 1;
    }
    if (e > 0) {
        if (e > 7) return 0;                 /* binomial(7,e) = 0 when e>7 */
        seq_t term = bin[e];
        if (e & 1) term = -term;
        result *= term;
    }

    /* odd factors */
    idx_t p;
    for (p = 3; p <= tmp / p; p += 2) {
        if (tmp % p != 0) continue;
        e = 0;
        while (tmp % p == 0) {
            e++;
            tmp /= p;
        }
        if (e > 7) return 0;
        seq_t term = bin[e];
        if (e & 1) term = -term;
        result *= term;
    }

    /* remaining prime > sqrt(original tmp) */
    if (tmp > 1) {
        e = 1;
        if (e > 7) return 0;
        seq_t term = bin[e];
        /* e is odd, so -term */
        if (e & 1) term = -term;
        result *= term;
    }

    return result;
}
