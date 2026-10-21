/*
 * A280913  Number of dots in International Morse numeral representation of n.
 * https://oeis.org/A280913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A280913 – Number of dots in the International Morse numeral representation of n.
   Formula: a(n) = ∑_{digits d of n} min(d, 10‑d)
   (for 0 ≤ d ≤ 9, min(d,10‑d) gives the dot count for the Morse numeral of d)
*/

seq_t a(idx_t n) {
    /* dot counts for decimal digits 0–9 in Morse numerals */
    static const unsigned dotcnt[10] = {0,1,2,3,4,5,4,3,2,1};
    seq_t sum = 0;

    /* Handle n = 0 as well as any positive n */
    do {
        unsigned digit = (unsigned)(n % 10);  /* last decimal digit, guaranteed <10 */
        sum += dotcnt[digit];
        n /= 10;
    } while (n != 0);

    return sum;
}
