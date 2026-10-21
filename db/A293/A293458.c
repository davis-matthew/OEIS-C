/*
 * A293458  Numerator of (Sum_{k=0..n} k!)/(Sum_{k=0..n} binomial(n,k)!).
 * https://oeis.org/A293458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, changed
 * Formula:   a(1)=1, a(2)=1; for n≥3 a(n) = ½·∑_{k=0}^{n} k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the definition: a(n) is the numerator of (∑ k!)/(∑ (C(n,k))!)
   and for n>2 the fraction reduces to a(n) = (∑ k!)/2.  */

seq_t a(idx_t n)
{
    /* Handle the first two indices explicitly. */
    if (n <= 2)
        return (seq_t)1;

    /* Compute the sum of factorials 0! + 1! + … + n!. */
    seq_t sum = (seq_t)0;
    seq_t fact = (seq_t)1;      /* 0! */
    sum += fact;                /* add 0! */

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        fact *= (seq_t)i;       /* i! = (i-1)!·i */
        sum += fact;
    }

    /* For n≥3 the desired numerator is half the sum. */
    return sum / (seq_t)2;
}
