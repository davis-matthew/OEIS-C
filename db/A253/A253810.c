/*
 * A253810  The life of Diophantus.
 * https://oeis.org/A253810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0) = 0; a(1) = 84/6 = 14 a(2) = 84/4 = 21 a(3) = 84*11/28 = 33
 *            a(4) = a(3)+5 = 38 a(5) = 84-4 = 80 a(6) = 84 a(n >= 6) = 84
 *            (constant beyond the last event)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const seq_t age = (seq_t)84;
    if (n == 0U)                 return (seq_t)0;
    else if (n == 1U)            return age / 6;
    else if (n == 2U)            return age / 4;
    else if (n == 3U)            return age * 11 / 28;
    else if (n == 4U)            return age * 11 / 28 + (seq_t)5;
    else if (n == 5U)            return age - (seq_t)4;
    else                         return age;   /* n >= 6 */
}
