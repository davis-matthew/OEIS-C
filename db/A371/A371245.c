/*
 * A371245  Number of factorizations of n into factors > 4.
 * https://oeis.org/A371245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into factors > 4.
 *            The trivial factorization consisting of the single factor n
 *            itself is counted. This implementation uses a recursive helper
 *            with a non‑decreasing factor order to avoid duplicate
 *            factorizations
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 108 terms through n = 9995 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _factor_count(idx_t n, idx_t min_factor) {
    if (n < min_factor)          /* no factor allowed */
        return 0;
    /* n is at least min_factor: count the trivial representation n itself */
    seq_t res = 1;
    /* iterate over possible first factors f >= min_factor, ensuring f <= n/f
       (i.e., f <= sqrt(n)) to avoid duplicates. */
    for (idx_t f = min_factor; f <= n / f; ++f) {
        if (n % f == 0) {
            res += _factor_count(n / f, f);  /* recursively factor the remainder */
        }
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 1)          /* by convention a(1) = 1 */
        return 1;
    if (n < 5)           /* 2, 3, 4 have no factorizations into factors > 4 */
        return 0;
    return _factor_count(n, 5);  /* start with the smallest allowed factor */
}
