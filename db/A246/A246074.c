/*
 * A246074  Paradigm Shift Sequence for a (-4,5) production scheme with
 *          replacement.
 * https://oeis.org/A246074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 85, u32 n <= 181, u64 n <= 373,
 *            u128 n <= 757, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n       for 1 <= n <= 12
   Recurrence: a(n) = 2*a(n-6) for n >= 13
   Using this recurrence the sequence is computed in O(n/6) time. */
seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;     /* current index in seq_t form */
    seq_t pow = 1;          /* multiplier 2^k */

    /* Bring the index down to the first 12 terms by subtracting 6
       and doubling the multiplier each time. */
    while (t > 12)
    {
        t -= 6;
        pow <<= 1;          /* pow = pow * 2 */
    }
    return pow * t;         /* final value */
}
