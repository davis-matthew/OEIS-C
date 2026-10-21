/*
 * A285870  a(n) = floor(n/2) - floor((n+1)/6), n >= 0.
 * https://oeis.org/A285870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 765, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A285870
   Formula: a(n) = floor(n/2) - floor((n+1)/6), n >= 0
   (Non-recursive arithmetic implementation) */

seq_t a(idx_t n)
{
    /* Compute floor(n/2) using integer division */
    idx_t half = n >> 1;          /* n/2 for unsigned n */

    /* Compute floor((n+1)/6) without overflow:
       floor((n+1)/6) = n/6 + ((n % 6) == 5 ? 1 : 0) */
    idx_t div6 = n / 6;
    if (n % 6 == 5)
        div6 += 1;

    /* Convert to seq_t (unsigned) and subtract */
    return (seq_t)half - (seq_t)div6;
}
