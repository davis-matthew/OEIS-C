/*
 * A397110  Upper (1,1/2) midsequence of (floor[n/2]^2) and (ceiling[n/3]^2);
 *          see Comments.
 * https://oeis.org/A397110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// A397110: Upper (1,1/2)-midsequence of (floor[n/2]^2) and (ceil[n/3]^2)
// a(n) = floor(n/2)^2 + ceil(ceil(n/3)^2 / 2)
//       = floor(n/2)^2 + (ceil(n/3)^2 + 1)/2
/*
 * The function below implements this formula using only integer
 * arithmetic.  No tables, no recursion, and no memoisation are
 * required as every term can be computed in O(1) time.
 */
seq_t a(idx_t n)
{
    /* floor(n/2) and its square */
    seq_t s = (seq_t)(n >> 1);
    seq_t s2 = s * s;

    /* ceil(n/3) and its square */
    seq_t t = (seq_t)((n + 2u) / 3u);   /* (n+2)/3 is ceil(n/3) for n>=0 */
    seq_t t2 = t * t;

    /* ceil(t2/2) = (t2 + 1)/2 because t2 is integer */
    seq_t half = (t2 + 1) / 2;

    return s2 + half;
}
