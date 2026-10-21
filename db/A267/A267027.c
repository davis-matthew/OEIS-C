/*
 * A267027  "Polyrhythmic sequence" P(3,4): numbers congruent to 1 mod 3
 *          (A016777) or 1 mod 4 (A016813).
 * https://oeis.org/A267027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 12*ceil(n/6) - c[n % 6]   (n >= 1)
 *              where c = {2,11,8,7,5,3} for remainders 0..5.
 * This gives the Polyrhythmic sequence P(3,4) as defined in OEIS A267027.
 */
seq_t a(idx_t n) {
    /* offsets for each class of n % 6 */
    static const seq_t c[6] = {2, 11, 8, 7, 5, 3};

    /* compute k = ceil(n / 6) using integer arithmetic */
    idx_t k = (n + 5) / 6;        /* works even for large n */

    /* 12 * k fits into seq_t for all n that produce values representable in seq_t */
    seq_t res = (seq_t)12 * (seq_t)k - c[n % 6];
    return res;
}
