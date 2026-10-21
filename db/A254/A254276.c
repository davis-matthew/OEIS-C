/*
 * A254276  Decimal expansion of Boltzmann constant in m^-1 K^-1 according to
 *          the 2019 SI system.
 * https://oeis.org/A254276
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   the n-th term of A254276 is the (n‑1)-th digit of the decimal
 *            expansion of 69 + 137004911879989/272115870842319 (the Boltzmann
 *            constant in m⁻¹ K⁻¹). Index 2 corresponds to the first digit of
 *            the integer part (the '6' in 69)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Return 0 for indices below the offset (n < 2) – behaviour below the
       offset does not matter. */
    if (n < 2) return 0;

    /* Position in the decimal expansion, 1‑based: pos= n‑1.  */
    idx_t pos = n - 1;

    /* Integer part of the constant.  */
    const seq_t I = (seq_t)69;
    const idx_t int_digits = 2;  /* 69 has two digits */

    if (pos <= int_digits) {
        /* Digit of the integer part: */
        idx_t exp = int_digits - pos;          /* power of 10 */
        seq_t div = 1;
        for (idx_t i = 0; i < exp; ++i) div *= 10;
        return (I / div) % 10;
    }

    /* Position inside the fractional part */
    idx_t after = pos - int_digits;  /* 1‑based decimal position */

    const seq_t p = (seq_t)137004911879989ull;
    const seq_t q = (seq_t)272115870842319ull;

    seq_t r = p;  /* current remainder */

    /* Advance the remainder to the desired decimal position.  */
    for (idx_t i = 1; i < after; ++i) {
        r = (r * 10) % q;
    }

    /* Compute the digit at position 'after'.  */
    r *= 10;
    seq_t digit = r / q;  /* digit is 0‑9 */
    return digit % 10;
}
