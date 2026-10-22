/*
 * A326300  Steinhaus sums.
 * https://oeis.org/A326300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} floor(n/2^k + 1/2)*2^k
 * Range:     u8 n <= 39, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t pow_half = 1;   /* 2^(k-1) */
    idx_t k = 1;

    /* Iterate while the term can be non‑zero and avoid UB on shifts */
    while (pow_half <= n && k <= 63) {
        /* floor( (n + 2^(k-1)) / 2^k ) == ((n + pow_half) >> k) */
        seq_t t = ((seq_t)n + pow_half) >> k;

        /* 2^k == pow_half * 2 */
        seq_t mul = t * pow_half;
        mul <<= 1;

        sum += mul;

        /* Prepare for next k, stop before overflow on pow_half */
        if (pow_half > ((idx_t)-1) >> 1)
            break;
        pow_half <<= 1;
        ++k;
    }
    return sum;
}
