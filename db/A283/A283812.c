/*
 * A283812  Poly-Bernoulli numbers B_n^(k) with k = -6.
 * https://oeis.org/A283812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 720*7^n - 1800*6^n + 1560*5^n - 540*4^n + 62*3^n - 2^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute x^exp for integer base x and non‑negative exponent exp. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Sequence A283812. */
seq_t a(idx_t n)
{
    /* Powers of the relevant bases. */
    seq_t p7 = pow_seq((seq_t)7, n);
    seq_t p6 = pow_seq((seq_t)6, n);
    seq_t p5 = pow_seq((seq_t)5, n);
    seq_t p4 = pow_seq((seq_t)4, n);
    seq_t p3 = pow_seq((seq_t)3, n);
    seq_t p2 = pow_seq((seq_t)2, n);

    seq_t result = 0;

    /* Positive contributions. */
    result += 720 * p7;
    result += 1560 * p5;
    result += 62  * p3;

    /* Negative contributions. */
    result -= 1800 * p6;
    result -= 540  * p4;
    result -= p2;

    return result;
}
