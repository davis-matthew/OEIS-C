/*
 * A363611  Expansion of Sum_{k>0} x^(4*k)/(1-x^k)^4.
 * https://oeis.org/A363611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 71, u32 n <= 2795, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A363611: a(n) = Sum_{d|n} binomial(d-1,3)  (offset 1) */

static inline seq_t binom3(idx_t d)
{
    /* Compute (d-1)(d-2)(d-3)/6 avoiding overflow by dividing
       by 2 and 3 before the multiplications. */
    seq_t a = d - 1;
    seq_t b = d - 2;
    seq_t c = d - 3;

    /* divide by 2 */
    if ((a & (seq_t)1) == 0)
        a /= 2;
    else if ((b & (seq_t)1) == 0)
        b /= 2;
    else
        c /= 2;

    /* divide by 3 */
    if (a % 3 == 0)
        a /= 3;
    else if (b % 3 == 0)
        b /= 3;
    else
        c /= 3;

    seq_t prod = a * b;
    prod *= c;          /* now prod equals binomial(d-1,3) */
    return prod;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)       /* iterate over divisors i of n */
    {
        if (n % i == 0)
        {
            if (i >= 4)
                sum += binom3(i);

            idx_t j = n / i;
            if (j != i && j >= 4)
                sum += binom3(j);
        }
    }
    return sum;
}
