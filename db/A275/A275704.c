/*
 * A275704  Digital root of n + (n+1)^2.
 * https://oeis.org/A275704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = digital_root( n + (n+1)^2 ) = A010888( n + (n+1)^2 )
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digital root can be computed using modulo 9 arithmetic.  */
seq_t a(idx_t n)
{
    /* Compute (n + (n+1)^2) mod 9 without overflow. */
    idx_t  n_mod   = n % 9;
    idx_t  np1_mod = (n + 1) % 9;
    idx_t  val_mod = (n_mod + (np1_mod * np1_mod) % 9) % 9;

    /* Digital root of a positive integer is 9 if the number is a multiple of 9. */
    return (seq_t)(val_mod ? val_mod : 9);
}
