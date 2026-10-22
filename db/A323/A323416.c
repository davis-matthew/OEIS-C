/*
 * A323416  a(n) = (n-1)! * (10^n - 1) / 9.
 * https://oeis.org/A323416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (n-1)! * (10^n - 1) / 9
 * Recurrence: a(1) = 1;  a(n+1) = n! * 10^n + n * a(n)
 */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, but guard against null input */
        return 0;

    seq_t a_val = 1;      /* a(1) */
    seq_t fact   = 1;      /* (i-1)! in the loop, starts as 0! */
    seq_t pow10  = 1;      /* 10^i in the loop, starts as 10^0 */

    for (idx_t i = 1; i < n; ++i) {
        fact *= (seq_t)i;          /* fact = i!        */
        pow10 *= (seq_t)10;        /* pow10 = 10^i    */
        a_val = fact * pow10 + (seq_t)i * a_val; /* a(i+1) */
    }

    return a_val;
}
