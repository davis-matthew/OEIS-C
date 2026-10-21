/*
 * A261691  Change of base from fractional base 3/2 to base 3.
 * https://oeis.org/A261691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{i≥0} (digit i of n in base 3/2) · 3^i The base‑3/2
 *            representation of n can be obtained by the recurrence: let c = n
 *            mod 3 (the least significant digit); then the remaining part is
 *            n' = (2·(n – c))/3. Repeating this gives all digits. The result
 *            a(n) is simply the same digits interprete...
 * Range:     u8 n <= 23, u16 n <= 185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t  sum   = 0;   /* accumulated result */
    seq_t  power = 1;   /* current 3^i value   */
    idx_t  cur   = n;   /* remaining part of n */

    while (cur > 0) {
        idx_t c = cur % 3;                   /* least‑significant digit   */
        sum += (seq_t)c * power;             /* add c·3^i to the result   */
        power *= 3;                          /* advance 3^i                */
        /* compute next part: n' = (2·(cur – c))/3  (exactly divisible) */
        cur = (unsigned __int128)2 * (cur - c) / 3;
    }

    return sum;
}
