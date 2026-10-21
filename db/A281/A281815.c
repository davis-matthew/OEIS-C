/*
 * A281815  Expansion of f(x, x^10) in powers of x where f(, ) is Ramanujan's
 *          general theta function.
 * https://oeis.org/A281815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 478, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A281815
   a(n) = 1 if n = k*(11*k+9)/2 or n = k*(11*k-9)/2 for some integer k ≥ 0,
   otherwise a(n) = 0.  The function assumes seq_t is a sufficiently wide
   unsigned integer type (uint8_t ... __int128, or arbitrary-precision). */

seq_t a(idx_t n) {
    /* Handle the constant term a(0) = 1. */
    if (n == 0)
        return 1;

    seq_t target = (seq_t)n;
    /* Iterate over k >= 1. For each k compute both candidate exponents. */
    for (idx_t k = 1; ; ++k) {
        seq_t k_se = (seq_t)k;
        seq_t v1 = k_se * ((seq_t)11 * k_se - 9) / 2;   /* k*(11k-9)/2 */
        seq_t v2 = k_se * ((seq_t)11 * k_se + 9) / 2;   /* k*(11k+9)/2 */

        if (v1 == target || v2 == target)
            return 1;

        /* Once both values exceed target we can stop; the forms are
           strictly increasing with respect to k. */
        if (v1 > target && v2 > target)
            break;
    }
    return 0;
}
