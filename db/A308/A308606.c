/*
 * A308606  Number of (not necessarily maximal) cliques in the n-transposition
 *          graph.
 * https://oeis.org/A308606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n^2 - n + 4) * n! / 4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* compute n! with seq_t, safe for large n if seq_t has biginteger capabilities */
static seq_t factorial(idx_t m) {
    seq_t f = 1;
    for (idx_t i = 2; i <= m; ++i) {
        f *= i;
    }
    return f;
}

seq_t a(idx_t n) {
    /* a(1) is the first term; values for n < 1 are undefined */
    if (n < 1) return 0;

    seq_t sn = (seq_t)n;
    seq_t coef = sn * sn - sn + 4;          /* n^2 - n + 4 */

    seq_t fact = factorial(n);             /* n! */
    seq_t fact_mod4 = fact % 4;            /* n! mod 4 */

    /* Determine the largest power of 2 dividing n! up to 4 */
    unsigned int g;
    switch ((uint64_t)fact_mod4) {
        case 0: g = 4; break;
        case 2: g = 2; break;
        default: g = 1; break;
    }

    seq_t fact_div = fact / g;              /* (n!)/g  */
    unsigned int div_by = 4 / g;            /* remaining divisor on coef   */
    seq_t coef_div = coef / div_by;         /* (n^2 - n + 4) / (4/g) */

    seq_t prod = fact_div * coef_div;       /* product fits in seq_t for n where result fits  */
    seq_t result = 1 + prod;                /* 1 + (...)/4 */

    return result;
}
