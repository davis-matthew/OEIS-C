/*
 * A258703  a(n) = floor(n/sqrt(2) - 1/2).
 * https://oeis.org/A258703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n*sqrt(2)-1)/2)
 * Range:     u8 n <= 361, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses the convergents of the continued fraction of sqrt(2)
   to avoid floating point arithmetic.  The recurrence for the convergents
   of sqrt(2) is:
         p(0)=1, q(0)=1
         p(1)=3, q(1)=2
         p(k+1)=2*p(k)+p(k-1),   q(k+1)=2*q(k)+q(k-1)
   For a given n, we use the first convergent with q>n.  Then
         floor(n*sqrt(2)) = floor(n*p/q)
   Finally a(n) = (floor(n*sqrt(2))-1)/2.  All arithmetic is performed
   on unsigned __int128 to keep the intermediate product safe for n up
   to at least 10,000. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1, but guard anyway */

    /* convergents of sqrt(2) */
    unsigned __int128 p_prev = (unsigned __int128)1;
    unsigned __int128 q_prev = (unsigned __int128)1;
    unsigned __int128 p_curr = (unsigned __int128)3;
    unsigned __int128 q_curr = (unsigned __int128)2;

    /* find the first convergent with denominator > n */
    while (q_curr <= n) {
        unsigned __int128 p_next = 2 * p_curr + p_prev;
        unsigned __int128 q_next = 2 * q_curr + q_prev;
        p_prev = p_curr;   q_prev = q_curr;
        p_curr = p_next;   q_curr = q_next;
    }

    /* floor(n*sqrt(2)) = floor(n*p_curr / q_curr) */
    unsigned __int128 prod     = (unsigned __int128)n * p_curr;
    unsigned __int128 floor_np = prod / q_curr;

    /* a(n) = (floor(n*sqrt(2)) - 1) / 2 */
    seq_t floor_val = (seq_t)floor_np;
    seq_t result    = (floor_val - (seq_t)1) >> 1;

    return result;
}
