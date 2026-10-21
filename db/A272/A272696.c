/*
 * A272696  Coxeter number for the reflection group E_n.
 * https://oeis.org/A272696
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Coxeter number of the reflection group E_n (for 3 ≤ n ≤
 *            8). a(3) = 6 (A₂ × A₁) a(4) = 5 (A₄) a(5) = 8 (D₅) a(6) = 12
 *            (E₆) a(7) = 18 (E₇) a(8) = 30 (E₈) For n > 8 the Coxeter number
 *            is infinite; we return 0
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) /* below offset */ return (seq_t)0;

    /* Piecewise values for 3 ≤ n ≤ 8 */
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)5;
    if (n == 5) return (seq_t)8;
    if (n == 6) return (seq_t)12;
    if (n == 7) return (seq_t)18;
    if (n == 8) return (seq_t)30;

    /* For n > 8 the Coxeter number is infinite */
    return (seq_t)0;
}
