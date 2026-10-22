/*
 * A321984  Decimal expansion of number of kilograms (kg) in 1 international
 *          avoirdupois ounce (oz).
 * https://oeis.org/A321984
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A321984: Decimal expansion of the number of kilograms in 1 oz.
   a(n) is the (n+1)-th digit of the integer 28349523125
   (index n = -1 gives the first digit, 2). */
seq_t a(idx_t n)
{
    /* The sequence is simply the decimal digits of 28349523125,
       starting with the first non–zero digit after the decimal point. */
    static const seq_t digits[11] = {2, 8, 3, 4, 9, 5, 2, 3, 1, 2, 5};
    if (n < -1)                 /* below the offset – undefined behaviour,
                                   we return 0 for safety */
        return 0;
    idx_t idx = n + 1;          /* position in the digits array (0‑based) */
    if (idx >= 11)              /* beyond the terminating decimal expansion */
        return 0;
    return digits[idx];
}
