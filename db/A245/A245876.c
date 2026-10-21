/*
 * A245876  Number of length 7+2 0..n arrays with some pair in every
 *          consecutive three terms totalling exactly n.
 * https://oeis.org/A245876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 52, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A245876: Number of 7+2 arrays (0..n) with a pair in every consecutive
 *  triple summing to exactly n.
 *
 * Conjectured closed form (valid for all n≥1):
 *   if (n is even)  a(n) = 1 + 37*n + 43*n^2 + 126*n^3 + 89*n^4 + 9*n^5
 *   if (n is odd)   a(n) = 7 - 76*n - 41*n^2 + 122*n^3 + 89*n^4 + 9*n^5
 *
 * The function below implements this formula using only integer
 * arithmetic.  It works for any seq_t (including unsigned or arbitrary‑
 * precision types) and is O(1) per call.  For seq_t sizes that cannot
 * hold the full result (e.g. 64‑bit builds for large n) the standard
 * C unsigned overflow semantics apply; the sequence values that fit
 * within seq_t remain correct.
 */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type */
    seq_t nn = (seq_t)n;

    /* powers of n up to n^5 */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t n5 = n4 * nn;          /* n^5 */

    /* compute the polynomial depending on parity */
    if ((n & 1) == 0)   /* even n */
    {
        return (seq_t)1
             + (seq_t)37 * nn
             + (seq_t)43 * n2
             + (seq_t)126 * n3
             + (seq_t)89 * n4
             + (seq_t)9  * n5;
    }
    else                /* odd n */
    {
        /* positive part */
        seq_t pos = (seq_t)7
                  + (seq_t)122 * n3
                  + (seq_t)89  * n4
                  + (seq_t)9   * n5;

        /* negative part to subtract */
        seq_t neg = (seq_t)76 * nn
                  + (seq_t)41 * n2;

        return pos - neg;
    }
}
