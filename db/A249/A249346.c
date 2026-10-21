/*
 * A249346  The exponent of the highest power of 6 dividing the product of the
 *          elements on the n-th row of Pascal's triangle.
 * https://oeis.org/A249346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hear, look
 * Formula:   a(n) = min( v_2(P(n)), v_3(P(n)) ) * where P(n) = product of the
 *            entries of the n‑th row of Pascal's triangle. * * v_2(P(n)) = 2
 *            * Σ_{k=0}^n s_2(k) - (n+1) * s_2(n) * v_3(P(n)) = (2 * Σ_{k=0}^n
 *            s_3(k) - (n+1) * s_3(n)) / 2 * * Here s_p(k) is the sum of the
 *            base‑p digits of k
 * Range:     u8 n <= 53, u16 n <= 7775, u32 n <= 7775, u64 n <= 7775,
 *            u128 n <= 7775, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t digit_sum(idx_t n, idx_t base)
{
    seq_t sum = 0;
    idx_t x = n;
    while (x > 0) {
        sum += x % base;
        x /= base;
    }
    return sum;
}

static seq_t sum_digits_to(idx_t n, idx_t base)
{
    seq_t total = 0;
    idx_t i;
    for (i = 0; i <= n; ++i)
        total += digit_sum(i, base);
    return total;
}

seq_t a(idx_t n)
{
    /* Convert n to seq_t once for arithmetic */
    seq_t nseq = (seq_t)n;
    seq_t n1 = nseq + 1;

    /* Base‑2 digit sums */
    seq_t s2n = digit_sum(n, (idx_t)2);
    seq_t sum2 = sum_digits_to(n, (idx_t)2);
    seq_t v2 = 2 * sum2 - n1 * s2n;

    /* Base‑3 digit sums */
    seq_t s3n = digit_sum(n, (idx_t)3);
    seq_t sum3 = sum_digits_to(n, (idx_t)3);
    seq_t numerator = 2 * sum3 - n1 * s3n;
    seq_t v3 = numerator / 2;  /* exact division: numerator is even */

    return (v2 < v3) ? v2 : v3;
}
