/*
 * A251780  Digital root of A069778(n-1) = n^3 - n^2 + 1, n >= 1. Repeat(1, 6,
 *          3, 7, 6, 6, 4, 6, 9).
 * https://oeis.org/A251780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = digital root of n^3 - n^2 + n
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 6, a(3) = 3, ... */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0. */
    if (n == 0)
        return 0;

    /* Work modulo 9.  All computations fit in native signed 64‑bit
       integers because the maximum value of n % 9 is 8. */
    uint64_t r  = n % 9;           /* n mod 9          */
    uint64_t r2 = (r * r) % 9;     /* n^2 mod 9        */
    uint64_t r3 = (r2 * r) % 9;    /* n^3 mod 9        */

    /* (n^3 - n^2 + n) mod 9 */
    int64_t temp = (int64_t)r3 - (int64_t)r2 + (int64_t)r;
    temp %= 9;
    if (temp < 0)
        temp += 9;

    /* Digital root: 0 → 9, otherwise the value itself. */
    return (temp == 0) ? (seq_t)9 : (seq_t)temp;
}
