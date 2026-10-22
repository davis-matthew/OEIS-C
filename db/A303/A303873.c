/*
 * A303873  Total area of the family of squares with side length n such that n
 *          = p + q, p divides q and p < q.
 * https://oeis.org/A303873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * Sum_{i=1..floor((n-1)/2)} (floor((n-i)/i) -
 *            floor((n-i-1)/i))
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified: a(n) = n^2 * (d(n) - 1 - (n%2==0 ? 1 : 0)) where d(n) is the number of divisors of n. */

/* Computes the number of divisors of n (n>=1). */
static seq_t _divisor_count(idx_t n)
{
    seq_t count = 1;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            count *= (seq_t)(exp + 1);
        }
    }
    if (temp > 1)
        count *= (seq_t)2;
    return count;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is at least 1 by definition. */
    seq_t nn = (seq_t)n;
    seq_t div_cnt = _divisor_count(n);

    /* d(n) - 1 - (n%2==0 ? 1 : 0) */
    seq_t lt_half = div_cnt - (seq_t)1;
    if ((n & (idx_t)1) == 0) {        /* n is even */
        if (lt_half == 0) return (seq_t)0;
        --lt_half;
    }
    if (lt_half == 0) return (seq_t)0;

    /* a(n) = n^2 * lt_half */
    return nn * nn * lt_half;
}
