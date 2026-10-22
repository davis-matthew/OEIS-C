/*
 * A353957  a(n) = (85*4^n - 1)/3.
 * https://oeis.org/A353957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (85*4^n - 1)/3 (offset 1)
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS sequence is defined for n >= 1; 0 is unused.  */
    if (n == 0u) return (seq_t)0;

    /* Compute 4^n using exponentiation by squaring.  The multiplication
       of seq_t values is defined for all seq_t types (built‑in integers
       or arbitrary‑precision integers).  */
    seq_t pow4 = (seq_t)1;
    seq_t base = (seq_t)4;
    idx_t exp = n;
    while (exp > 0u) {
        if (exp & 1u) pow4 = pow4 * base;
        base = base * base;
        exp >>= 1u;
    }

    /* a(n) = (85 * 4^n - 1) / 3  */
    seq_t res = (seq_t)85 * pow4;
    res = (res - (seq_t)1) / (seq_t)3;
    return res;
}
