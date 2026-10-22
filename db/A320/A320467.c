/*
 * A320467  Two-column table read by rows: The Mayan 260-day Tzolkin cycle,
 *          with day names replaced by numbers.
 * https://oeis.org/A320467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, tabf
 * Formula:   a(2n-1) = ((n - 1) mod 13) + 1; a(2n) = ((n - 1) mod 20) + 1;
 * Range:     u8 n <= 520, u16 n <= 520, u32 n <= 520, u64 n <= 520,
 *            u128 n <= 520, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* out of range, undefined for the sequence */

    if (n & 1) { /* odd n: 2k-1 */
        idx_t t = (n - 1) >> 1;          /* t = (n-1)/2 = k-1 */
        return (seq_t)(t % 13) + 1;      /* ((k-1) mod 13)+1 */
    } else { /* even n: 2k */
        idx_t t = (n >> 1) - 1;          /* t = n/2 - 1 = k-1 */
        return (seq_t)(t % 20) + 1;      /* ((k-1) mod 20)+1 */
    }
}
