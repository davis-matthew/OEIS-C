/*
 * A286144  Compound filter: a(n) = T(A000010(n), A257993(n)), where T(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A286144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(2 + ((A000010(n)+A257993(n))^2) - A000010(n) -
 *            3*A257993(n))
 * Range:     u8 n <= 28, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9661 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _a257(idx_t n)
{
    if ((n & (idx_t)1) != 0)
        return (seq_t)1;                /* odd n */
    if ((n % (idx_t)6) != 0)
        return (seq_t)2;                /* even, not divisible by 3 */
    if ((n % (idx_t)30) == 0)
        return (seq_t)4;                /* divisible by 30 */
    return (seq_t)3;                    /* even, divisible by 6 but not 30 */
}
/* Uses A000010(n)=phi(n) and a special definition of A257993(n) described below. */

static seq_t _phi(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;
    seq_t result = (seq_t)n;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            result -= result / p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        result -= result / temp;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n < 1 */
    seq_t phi  = _phi(n);
    seq_t a257 = _a257(n);
    seq_t sum  = phi + a257;
    seq_t val  = (((seq_t)2 + sum * sum - phi - (seq_t)3 * a257) / 2);
    return val;
}
