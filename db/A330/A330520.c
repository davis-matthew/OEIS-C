/*
 * A330520  Sum of even integers <= n times the sum of odd integers <= n.
 * https://oeis.org/A330520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor(n/2) * (floor(n/2) + 1)) * (ceil(n/2)^2)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 2;          /* floor(n/2)   */
    idx_t m = (n + 1) / 2;    /* ceil(n/2)    */
    seq_t km = (seq_t)k;
    seq_t mm = (seq_t)m;
    seq_t even = km * (km + 1); /* sum of even numbers <= n */
    seq_t odd  = mm * mm;       /* sum of odd numbers  <= n */
    return even * odd;
}
