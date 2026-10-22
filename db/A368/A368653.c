/*
 * A368653  Decimal expansion of 58291/21444.
 * https://oeis.org/A368653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368653: decimal expansion of 58291/21444
   a(1) = 2 (the integer part)
   For n > 1, a(n) is the (n-1)-th digit after the decimal point. */

/* The fraction 58291/21444 is stored in 64‑bit constants: */
#define NUM 58291ULL
#define DEN 21444ULL

seq_t a(idx_t n)
{
    /* first digit is the integer part of the fraction */
    if (n == 1) return (seq_t)(NUM / DEN);

    /* for n > 1 we compute the (n-1)-th decimal digit */
    idx_t   i;      /* loop counter */
    uint64_t r = NUM % DEN;      /* current remainder, fits in 64 bits */

    for (i = 0; i < n - 1; ++i)
    {
        uint64_t tmp = r * 10ULL;      /* 0 <= r < DEN, tmp < 10*DEN < 2^32 */
        seq_t d = (seq_t)(tmp / DEN);  /* digit for this position */
        r       = tmp % DEN;           /* new remainder for next step */
        if (i == n - 2) return d;      /* return digit at position n */
    }

    /* This point should never be reached, but provide a safe return */
    return (seq_t)0;
}
