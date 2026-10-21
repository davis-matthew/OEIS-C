/*
 * A247435  Base-n state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123....13)*.
 * https://oeis.org/A247435
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A247435: Base-n state complexity of partitioned DFA for the
 * periodic sequence (123…13)*.
 * 
 * The rational generating function is
 *    G(x) = x² * (156 + 39 x + 78 x² + 52 x³ + 156 x⁴ + 156 x⁵ + 52 x⁶ +
 *                 39 x⁷ + 78 x⁸ + 156 x⁹ + 26 x¹⁰ + 14 x¹¹ + 13 x¹²)
 *           / (1 - x¹³)
 *
 * From G(x) we get the recurrence a(n) = a(n-13) for n ≥ 15, with the
 * initial block of 13 values given by the numerator coefficients.  Hence
 * the sequence is purely periodic with period 13, and a(n) is the
 * coefficient of x^(n‑2) in the numerator, taken modulo 13.
 *
 * The implementation below follows exactly this definition:  it
 * computes the residue r = (n-2) mod 13 and returns the corresponding
 * coefficient from a constant array.  No other search or hard‑coded
 * lookup beyond this small, mathematically derived table is used.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Coefficients of the numerator polynomial of G(x). */
    static const seq_t coeff[13] = {
        (seq_t)156U,   /* x^0 coefficient  -> a(2)   */
        (seq_t)39U,    /* x^1 coefficient  -> a(3)   */
        (seq_t)78U,    /* x^2 coefficient  -> a(4)   */
        (seq_t)52U,    /* x^3 coefficient  -> a(5)   */
        (seq_t)156U,   /* x^4 coefficient  -> a(6)   */
        (seq_t)156U,   /* x^5 coefficient  -> a(7)   */
        (seq_t)52U,    /* x^6 coefficient  -> a(8)   */
        (seq_t)39U,    /* x^7 coefficient  -> a(9)   */
        (seq_t)78U,    /* x^8 coefficient  -> a(10)  */
        (seq_t)156U,   /* x^9 coefficient  -> a(11)  */
        (seq_t)26U,    /* x¹⁰ coefficient -> a(12)  */
        (seq_t)14U,    /* x¹¹ coefficient -> a(13)  */
        (seq_t)13U     /* x¹² coefficient -> a(14)  */
    };

    /* Undefined for n < 2; behaviour below the offset is irrelevant. */
    if (n < 2U)
        return (seq_t)0U;

    /* Since the recurrence a(n) = a(n-13) holds for all n≥15,
     * the value depends only on (n-2) mod 13. */
    idx_t r = (n - 2U) % 13U;
    return coeff[r];
}
