/*
 * A318972  The 7x+-1 function ("shortcut" definition): a(n) = (7n+1)/4 if n
 *          == +1 (mod 4), a(n) = (7n-1)/4 if n == -1 (mod 4), otherwise a(n)
 *          = n/2.
 * https://oeis.org/A318972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 1)/4 if n == +1 (mod 4), a(n) = (7*n - 1)/4 if n
 *            == -1 (mod 4), a(n) = n/2 otherwise
 * Range:     u8 n <= 146, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    unsigned rem = n & 3ULL;          /* n mod 4 */
    seq_t x = n;                      /* convert index to seq_t */
    if (rem == 1) {
        unsigned __int128 t = (unsigned __int128)x * 7 + 1;
        return (seq_t)(t / 4);
    } else if (rem == 3) {
        unsigned __int128 t = (unsigned __int128)x * 7 - 1;
        return (seq_t)(t / 4);
    } else {
        return x >> 1;                /* n/2 */
    }
}
