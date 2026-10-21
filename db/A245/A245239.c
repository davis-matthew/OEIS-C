/*
 * A245239  Maximum frustration of complete bipartite graph K(n,6).
 * https://oeis.org/A245239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(66/32*n) - 1 if n = 6 or n ≡ 5 (mod 8) or n ≡
 *            2,4,7,9,11 (mod 16) or n ≡ 10,15,16,24 (mod 32); a(n) =
 *            floor(66/32*n) - 2 if n ≡ 1,3,17,19 (mod 32); a(n) =
 *            floor(66/32*n) otherwise
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(33*n/16) using integer division. */
    seq_t base = ((seq_t)n * 33u) / 16;
    seq_t sub = 0;

    if (n == 6 || n % 8u == 5u) {
        sub = 1;
    } else if (n % 16u == 2u || n % 16u == 4u ||
               n % 16u == 7u || n % 16u == 9u ||
               n % 16u == 11u) {
        sub = 1;
    } else if (n % 32u == 1u || n % 32u == 3u ||
               n % 32u == 17u || n % 32u == 19u) {
        sub = 2;
    } else if (n % 32u == 10u || n % 32u == 15u ||
               n % 32u == 16u || n % 32u == 24u) {
        sub = 1;
    }

    return base - sub;
}
