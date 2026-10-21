/*
 * A278182  Number of dots in Maya numeral representation of n.
 * https://oeis.org/A278182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of (digit modulo 5) over all Maya digits of n * where
 *            digits are obtained by converting n to the Maya base * sequence
 *            [20, 18, 20, 20, ...] (base 20 for the first and all *
 *            subsequent places, but place 1 uses base 18 due to the Maya *
 *            3rd‑place weight 360 = 18 × 20)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t temp = n;
    seq_t dots = 0;
    idx_t base = 20;       /* initial base for units place          */
    idx_t place = 0;       /* counts the current place              */
    do {
        idx_t digit = temp % base;
        dots += (seq_t)(digit % 5);   /* count dots in this digit          */
        temp /= base;                 /* move to next more significant    */
        if (place == 0)               /* second place uses base 18         */
            base = 18;
        else
            base = 20;                /* all higher places use base 20     */
        place++;
    } while (temp > 0);
    return dots;
}
