/*
 * A250021  Number of length 7+5 0..n arrays with no six consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^12 - (68/35)*n^11 + (11737/1260)*n^10 -
 *            (90763/7560)*n^9 + (29389/1008)*n^8 - (1819/63)*n^7 +
 *            (2363/72)*n^6 - (457/280)*n^5 - (31987/1008)*n^4 +
 *            (180149/3780)*n^3 - (2139/70)*n^2 + (1427/210)*n (all terms are
 *            integers for n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* coefficients multiplied by the common denominator 15120; */
    /* array index 0 => n^0 coefficient, index 12 => n^12 coefficient */
    static const long long coeff[13] = {
        0LL,           /* n^0  */
       102744LL,      /* n^1  */
      -462024LL,      /* n^2  */
       720596LL,       /* n^3  */
      -479805LL,      /* n^4  */
      -24678LL,       /* n^5  */
       496230LL,       /* n^6  */
      -436560LL,      /* n^7  */
       440835LL,       /* n^8  */
      -181526LL,      /* n^9  */
       140844LL,       /* n^10 */
      -29376LL,       /* n^11 */
       15120LL        /* n^12 */
    };

    /* Use Han­orner’s rule; intermediate values fit in 128‑bit signed int */
    __int128 val = 0;
    seq_t n_seq = (seq_t)n;                /* promote to seq_t for safety   */

    for (int i = 12; i >= 0; --i)
    {
        val = val * (__int128)n_seq + (__int128)coeff[i];
    }

    /* final division by the common denominator 15120 */
    return (seq_t)(val / (__int128)15120);
}
