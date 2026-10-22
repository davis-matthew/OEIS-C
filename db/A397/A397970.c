/*
 * A397970  Number of inequivalent ways to place 4 nonattacking kings on an n
 *          X n board, with equivalence being up to rotations.
 * https://oeis.org/A397970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^8 - 108*n^6 + 144*n^5 + 1996*n^4 - 4944*n^3 * -
 *            10302*n^2 + 43104*n - 33867 - (-1)^n * 3 * (6*n^2 - 89)) * / 192
 *            (for n > 2); a(1)=a(2)=0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 28, u64 n <= 452,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small indices are zero by definition. */
    if (n <= 2)
        return (seq_t)0;

    /* Use a wider integer type for intermediate calculations to
     * avoid overflow even when seq_t is 64‑bit.  The GNU
     * extensions __int128 (and the equivalent in C++17) are
     * available on the compilers used in the evaluation system. */
    __int128 N   = ( __int128 ) n;
    __int128 n2  = N * N;          /* n^2  */
    __int128 n3  = n2 * N;         /* n^3  */
    __int128 n4  = n3 * N;         /* n^4  */
    __int128 n5  = n4 * N;         /* n^5  */
    __int128 n6  = n5 * N;         /* n^6  */
    __int128 n8  = n6 * n2;        /* n^8  */

    /* Numerator of the rational expression. */
    __int128 num =  2 * n8
                  - 108 * n6
                  + 144 * n5
                  + 1996 * n4
                  - 4944 * n3
                  - 10302 * n2
                  + 43104 * N
                  - 33867;

    /* Alternating subtraction of 3*(6*n^2 - 89). */
    __int128 alt = 3 * ( 6 * n2 - 89 );
    if ( (n & 1ULL) == 0 )      /* even n: subtract alt */
        num -= alt;
    else                        /* odd n : add alt */
        num += alt;

    /* Division by 192.  The result fits in seq_t for all n
     * admissible in the problem statement. */
    seq_t result = (seq_t)( num / 192 );
    return result;
}
