/*
 * A397267  Upper (1/2,1) midsequence of (floor[n/2]^2) and (ceiling[n/3]^2);
 *          see Comments.
 * https://oeis.org/A397267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( (1/2)*floor(n/2)^2 + ceil(n/3)^2 )
 * Range:     u8 n <= 23, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*               = ((floor(n/2)^2 + 2*ceil(n/3)^2) + 1) / 2 */
seq_t a(idx_t n)
{
    /* floor(n/2) and ceil(n/3) as integers */
    idx_t f = n / 2;
    idx_t c = (n + 2) / 3;          /* (n+2)/3 == ceil(n/3) for integers */

    /* Compute squares in seq_t */
    seq_t sq_f = (seq_t)f * (seq_t)f;       /* floor(n/2)^2   */
    seq_t sq_c = (seq_t)c * (seq_t)c;       /* ceil(n/3)^2    */

    /* Sum: floor^2 + 2*ceil^2 */
    seq_t sum = sq_f + ((seq_t)2 * sq_c);

    /* Ceil of sum/2: (sum+1)/2 for integer types */
    return (sum + 1) / 2;
}
