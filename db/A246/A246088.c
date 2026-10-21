/*
 * A246088  Paradigm shift sequence for (2,2) production scheme with
 *          replacement.
 * https://oeis.org/A246088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 73, u32 n <= 153, u64 n <= 313,
 *            u128 n <= 633, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A246088: Paradigm‑shift sequence for (2,2) production scheme with replacement.
    Formula:  a(n) = Σ_{k=0}^{⌊n/10⌋} 4^k · p[n‑10k],
    where p[j] are the coefficients (index j=0..31) of the numerator polynomial
    N(x) = x·(1+2x+3x²+4x³+5x⁴+6x⁵+7x⁶+8x⁷+9x⁸+10x⁹+7x¹⁰+4x¹¹+3x¹²+2x¹³+x¹⁴+x²⁰+6x²¹+3x²²+2x²⁹+9x³⁰). */

/* Coefficient array for N(x) (degree 31). */
static const idx_t p[32] = {
    0,   /* x^0   */
    1,   /* x^1   */
    2,   /* x^2   */
    3,   /* x^3   */
    4,   /* x^4   */
    5,   /* x^5   */
    6,   /* x^6   */
    7,   /* x^7   */
    8,   /* x^8   */
    9,   /* x^9   */
    10,  /* x¹⁰  */
    7,   /* x¹¹  */
    4,   /* x¹²  */
    3,   /* x¹³  */
    2,   /* x¹⁴  */
    1,   /* x¹⁵  */
    0,   /* x¹⁶  */
    0,   /* x¹⁷  */
    0,   /* x¹⁸  */
    0,   /* x¹⁹  */
    0,   /* x²⁰  */
    1,   /* x²¹  */
    6,   /* x²²  */
    3,   /* x²³  */
    0,   /* x²⁴  */
    0,   /* x²⁵  */
    0,   /* x²⁶  */
    0,   /* x²⁷  */
    0,   /* x²⁸  */
    0,   /* x²⁹  */
    2,   /* x³⁰  */
    9    /* x³¹  */
};

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow4   = 1;               /* 4^k for current k */
    idx_t kmax   = n / 10;          /* floor(n/10) – maximum k */
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t idx = n - 10 * k;     /* index into p[] */
        if (idx <= 31)
            result += pow4 * (seq_t)p[idx];
        pow4 *= 4;                  /* next power of 4 */
    }
    return result;
}
