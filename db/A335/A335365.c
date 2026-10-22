/*
 * A335365  Numbers that are unreachable by the process of starting from 1 and
 *          adding 5 and/or multiplying by 3.
 * https://oeis.org/A335365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n <= 12) ? table[n-1] : 5*(n-6)
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows the OEIS definition: for n > 12, a(n) = 2*a(n-1) - a(n-2) */
/* For n > 10, the sequence is linear: a(n) = 5*(n-6), which holds for n >= 11. */

seq_t a(idx_t n)
{
    /* Table of the first 12 terms, indexed from 0 (n=1 -> idx 0). */
    static const seq_t first12[] = {
        2, 4, 5, 7, 10, 12, 15, 17, 20, 22, 25, 30
    };
    if (n == 0) {           /* Undefined for offset 0, return 0 as fallback. */
        return 0;
    }
    if (n <= 12) {
        return first12[n - 1];
    }
    /* For n >= 13, use the linear formula 5*(n-6). */
    return (seq_t)5 * ((seq_t)n - (seq_t)6);
}
