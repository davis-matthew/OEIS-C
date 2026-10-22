/*
 * A355678  For any nonnegative number n with factorial base expansion Sum_{k
 *          > 0} d_k * k!, a(n) = Sum_{k > 0} d_k * k! * (-1)^(Sum_{i < k}
 *          sign(d_i)).
 * https://oeis.org/A355678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Sum_{k>0} d_k * k! * (-1)^(Sum_{i<k} sign(d_i)), where
 *            the d_k are the factorial‑base digits of n
 * Range:     i8 n <= 238, i16 n <= 5040, i32 n <= 5040, i64 n <= 5040,
 *            i128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_K 25   /* sufficient for 64‑bit n and arbitrary‑precision seq_t */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)          /* n=0 has all digits zero, so a(0)=0 */
        return 0;

    /* Pre‑compute factorials up to the first one exceeding n. */
    seq_t fact[ MAX_K + 1 ];
    uint8_t  d[ MAX_K + 1 ];

    fact[0] = (seq_t)1;   /* 0! = 1 */
    idx_t k = 1;
    while (k < MAX_K)
    {
        fact[k] = fact[k-1] * (seq_t)k;   /* k! = (k-1)! * k */
        if (fact[k] > (seq_t)n)
            break;
        ++k;
    }
    idx_t m = k - 1;       /* highest factorial index needed */

    /* Convert n to factorial base, storing digits in d[1..m] */
    seq_t rem = (seq_t)n;
    for (idx_t i = m; i > 0; --i)
    {
        d[i] = (uint8_t)(rem / fact[i]);   /* quotient is the digit */
        rem -= (seq_t)d[i] * fact[i];      /* remainder for next step */
    }

    /* Compute the alternating signed sum. */
    seq_t result = (seq_t)0;
    idx_t parity = 0;          /* parity of non‑zero digits seen so far */

    for (idx_t i = 1; i <= m; ++i)
    {
        if (d[i] != 0)
        {
            if (parity == 0)
                result += (seq_t)d[i] * fact[i];
            else
                result -= (seq_t)d[i] * fact[i];
            parity ^= 1;   /* toggle parity for the next higher digit */
        }
    }

    return result;
}
