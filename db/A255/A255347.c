/*
 * A255347  a(n) = n * (1 - (-1)^(n/4) / 4) if n divisible by 4, a(n) = n
 *          otherwise.
 * https://oeis.org/A255347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 211, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A255347
 * Formula: a(n) = n   if n%4 != 0
 *           a(n) = n * (3/4) if n%4 == 0 and (n/4)%2 == 0
 *           a(n) = n * (5/4) if n%4 == 0 and (n/4)%2 == 1
 */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* promote index to seq_t */
    if (n % 4 == 0) {
        idx_t k = n / 4;
        if ((k & 1) == 0)        /* k even -> factor 3/4 */
            x = x * 3 / 4;
        else                    /* k odd  -> factor 5/4 */
            x = x * 5 / 4;
    }
    return x;
}
