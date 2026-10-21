/*
 * A292531  a(n) = 0 if n is a power of 2. Otherwise, product of 2 numbers
 *          nearest n that have more 2's in their prime factorization than n.
 * https://oeis.org/A292531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is a power of 2; otherwise a(n) = p^2 * ceil(n/p)
 *            * floor(n/p), where p = 2^(v2(n)+1) and v2(n) is the exponent of
 *            2 in n
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* 0 for powers of 2 (including n==1). */
    if ((n & (n - 1)) == 0) {
        return 0;
    }

    /* Count the exponent of 2 in n (v2). */
    idx_t tmp = n;
    idx_t v2 = 0;
    while ((tmp & 1ULL) == 0) {
        ++v2;
        tmp >>= 1;
    }

    /* Compute p = 2^(v2+1). */
    seq_t p = 1;
    p <<= (unsigned)(v2 + 1);   /* shift count is cast to unsigned */

    /* Convert n to seq_t for later arithmetic. */
    seq_t sn = (seq_t)n;

    /* Compute ceil(n/p) and floor(n/p). */
    seq_t ceil_val = (sn + p - 1) / p;
    seq_t floor_val = sn / p;

    /* a(n) = p^2 * ceil_val * floor_val. */
    seq_t result = p * p;      /* p^2 */
    result *= ceil_val;
    result *= floor_val;

    return result;
}
