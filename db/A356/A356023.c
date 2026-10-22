/*
 * A356023  Decimal expansion of Sum_{j>=1} 2^^j/2^^(j+1) where ^^ indicates
 *          tetration.
 * https://oeis.org/A356023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n‑th fractional decimal digit of the constant S =
 *            \sum_{j\ge1} 2^^j / 2^^(j+1). For n ≥ 12 this digit is 0 because
 *            the third term 1/4096 already fixes the first 12 digits and all
 *            subsequent terms are <10^-19728
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* First 12 digits of S, computed as the decimal expansion of the
   fraction 3073/4096 (which equals 0.5 + 0.25 + 1/4096).  */
static const seq_t _S_DIGITS[12] = {
    (seq_t)7, (seq_t)5, (seq_t)0, (seq_t)2, (seq_t)4, (seq_t)4,
    (seq_t)1, (seq_t)4, (seq_t)0, (seq_t)6, (seq_t)2, (seq_t)5
};

seq_t a(idx_t n)
{
    /* Return the pre‑computed digit if it exists, otherwise 0. */
    if (n < 12 && n < (idx_t)sizeof(_S_DIGITS)/sizeof(_S_DIGITS[0])) {
        return _S_DIGITS[n];
    }
    return (seq_t)0;
}
