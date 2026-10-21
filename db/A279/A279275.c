/*
 * A279275  Numbers k such that 2*k+1 and 5*k+1 are both pentagonal numbers
 *          (A000326).
 * https://oeis.org/A279275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=500, a(2)=721525, a(3)=1040439075,
   a(n)=1443*a(n-1) - 1443*a(n-2) + a(n-3) for n>3 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)500;
    if (n == 2) return (seq_t)721525;
    if (n == 3) return (seq_t)1040439075;

    seq_t a1 = (seq_t)500;
    seq_t a2 = (seq_t)721525;
    seq_t a3 = (seq_t)1040439075;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t temp = a3 * (seq_t)1443;
        temp -= a2 * (seq_t)1443;
        temp += a1;
        a1 = a2;
        a2 = a3;
        a3 = temp;
    }
    return a3;
}
