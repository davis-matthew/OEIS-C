/*
 * A249079  a(n) = 29*n + floor( n/29 ) + 0^( 1-floor( (14+(n mod 29))/29 ) ).
 * https://oeis.org/A249079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29*n + floor( n/29 ) + 0^( 1 - floor( (14 + (n mod 29)) /
 *            29 ) )
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 29*n + floor(n/29) */
    seq_t value = (seq_t)n * 29 + (seq_t)n / 29;

    /* 0^(1 - floor((14 + n mod 29)/29)) equals
       0 when (n mod 29) ≤ 14, otherwise 1.
       Add 1 only when (n mod 29) ≥ 15. */
    if ((n % 29) > 14) {
        value += 1;
    }

    return value;
}
