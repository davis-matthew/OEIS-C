/*
 * A325829  "Sloping nonary numbers": write numbers in nonary under each other
 *          (right-justified), read diagonals in upward direction, convert to
 *          decimal.
 * https://oeis.org/A325829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n+1) - ((n+1) mod 9) + (n mod 9)
 * Range:     u8 n <= 240, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: write n+1 in base‑9, replace its least significant digit by that of n,
   interpret the result as base‑9, and return the decimal value. */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n1    = n_seq + (seq_t)1;
    seq_t res   = (n1 - (n1 % (seq_t)9)) + (n_seq % (seq_t)9);
    return res;
}
