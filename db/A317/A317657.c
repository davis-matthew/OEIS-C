/*
 * A317657  Numbers congruent to {15, 75, 95} mod 100.
 * https://oeis.org/A317657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + 100, a(1)=15, a(2)=75, a(3)=95 * Equivalent
 *            closed form: * a(n) = 15 + 100 * floor((n-1)/3) if n ≡ 1 (mod 3)
 *            * a(n) = 75 + 100 * floor((n-1)/3) if n ≡ 2 (mod 3) * a(n) = 95
 *            + 100 * floor((n-1)/3) if n ≡ 0 (mod 3)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = floor((n-1)/3) – use seq_t arithmetic to avoid overflow */
    seq_t base = ((seq_t)n - 1) / 3;

    switch (n % 3)
    {
    case 1:  /* n ≡ 1 (mod 3) */
        return (seq_t)15 + base * 100;
    case 2:  /* n ≡ 2 (mod 3) */
        return (seq_t)75 + base * 100;
    default: /* n ≡ 0 (mod 3) */
        return (seq_t)95 + base * 100;
    }
}
