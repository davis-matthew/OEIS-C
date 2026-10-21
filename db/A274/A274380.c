/*
 * A274380  The period 4 sequence of the iterated sum of deficient divisors
 *          function (A187793) starting at 34.
 * https://oeis.org/A274380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = a(n). Initial terms (offset 1): a(1)=34, a(2)=54,
 *            a(3)=42, a(4)=48
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is periodic with period 4.  Store the first cycle
       in a static array and return the appropriate element.  */
    static const seq_t cycle[4] = {34, 54, 42, 48};
    /* Convert the 1‑based index to a 0‑based index modulo 4. */
    return cycle[(n - 1) % 4];
}
