/*
 * A321499  Numbers of the form (x - y)(x^2 - y^2) with x > y > 0.
 * https://oeis.org/A321499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321499: integers of the form (x‑y)(x²‑y²) with x > y > 0.
   For n = 1,2,3: 3, 5, 7.
   For n > 3:
     if n ≡ 3 (mod 5):   a(n) = 8 * ((n + 2) / 5)
     otherwise:          a(n) = ((((n + 2) * 4) / 5) * 2) + 1
*/
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)7;

    if (n % 5 == 3) {
        /* n = 5k – 2, k > 1 */
        idx_t k = (n + 2) / 5;
        return (seq_t)8 * (seq_t)k;
    }

    seq_t temp = (((seq_t)n + (seq_t)2) * (seq_t)4) / (seq_t)5;
    return temp * (seq_t)2 + (seq_t)1;
}
