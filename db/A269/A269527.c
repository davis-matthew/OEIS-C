/*
 * A269527  a(n) = n^20 + n^15 + n^10 + n^5 + 1.
 * https://oeis.org/A269527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^20 + n^15 + n^10 + n^5 + 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute x^e for small exponent e (<=20) using repeated multiplication.
 * The helper is static and not exposed outside this translation unit. */
static seq_t power(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        result *= base;
        exp--;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t nval = (seq_t)n;

    seq_t p5  = power(nval, 5);
    seq_t p10 = power(nval, 10);
    seq_t p15 = power(nval, 15);
    seq_t p20 = power(nval, 20);

    return p20 + p15 + p10 + p5 + (seq_t)1;
}
