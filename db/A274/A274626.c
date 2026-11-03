/*
 * A274626  a(n) = Product_{i=0..2} (2^floor((n+i)/3)-1).
 * https://oeis.org/A274626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=0}^{2} ( 2^{⌊(n+i)/3⌋} - 1 )
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 812 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_of_two_minus_one(idx_t exp)
{
    seq_t value = 1;
    while (exp--) {
        value <<= 1;
    }
    /* value = 2^exp now */
    value -= 1;  /* 2^exp - 1 */
    return value;
}

seq_t a(idx_t n)
{
    /* Compute the three exponents floor((n+i)/3) for i = 0,1,2 */
    idx_t e0 = n / 3;
    idx_t e1 = (n + 1) / 3;
    idx_t e2 = (n + 2) / 3;

    seq_t t0 = power_of_two_minus_one(e0);
    seq_t t1 = power_of_two_minus_one(e1);
    seq_t t2 = power_of_two_minus_one(e2);

    return t0 * t1 * t2;
}
