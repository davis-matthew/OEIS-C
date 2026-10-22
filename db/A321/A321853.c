/*
 * A321853  a(n) is the sum of the fill times of all 1-dimensional fountains
 *          given by the permutations in S_n.
 * https://oeis.org/A321853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{k=0..n} (n-k)*k/(k+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety, handle the base case explicitly. */
    if (n == 0)
        return (seq_t)0;

    /* Compute n! in sequential multiplication. */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* Compute the sum term by term, ensuring all divisions are exact. */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k < n; ++k) {
        seq_t numer = (seq_t)(n - k) * (seq_t)k;   /* (n-k)*k   */
        if (numer == (seq_t)0)                    /* skip zero terms */
            continue;
        seq_t div = fact / (seq_t)(k + 1);        /* n! / (k+1), exact */
        sum += numer * div;                       /* accumulate */
    }

    return sum;
}
