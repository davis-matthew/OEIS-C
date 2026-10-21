/*
 * A261557  a(0) = a(1) = 0; for n>1, a(n) = 2*n - a(n-1) - a(n-2).
 * https://oeis.org/A261557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/3) + (n mod 3 == 2 ? 4 : 0)
 * Range:     u8 n <= 379, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence equivalent: a(0)=a(1)=0, a(2)=4;  a(n)=a(n-3)+2 for n>2 */
seq_t a(idx_t n)
{
    /* Compute n/3 and n%3 in the caller's integer type.
       The intermediate 2*(n/3) fits in seq_t whenever a(n) does. */
    idx_t rem   = n % 3;
    seq_t q     = (seq_t)(n / 3);   /* floor(n/3) */
    seq_t ans   = q * 2;            /* 2 * floor(n/3) */
    if (rem == 2)
        ans += 4;                   /* add the base term for residue 2 */
    return ans;
}
