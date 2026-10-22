/*
 * A334566  Number of solutions of the Diophantine equation z^2 - y^2 - x^2 =
 *          n > 0 when the positive integers, x, y and z, are consecutive
 *          terms of an arithmetic progression.
 * https://oeis.org/A334566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7814, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count ordered divisor pairs (a,b) of n such that
   a*b = n, (a+b) ≡ 0 (mod 4), and b < 3a.  Each such pair
   corresponds to a distinct solution (x, y, z) of
   z² - y² - x² = n with x,y,z consecutive terms of an arithmetic
   progression. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t a = 1; a <= n / a; ++a)
    {
        if (n % a == 0)
        {
            idx_t b = n / a;
            seq_t sum = (seq_t)a + (seq_t)b;

            if (sum % 4 == 0)
            {
                /* first orientation: (a,b) */
                if ((seq_t)b < (seq_t)a * 3)
                    ++count;

                /* second orientation: (b,a), if distinct */
                if (a != b)
                {
                    if ((seq_t)a < (seq_t)b * 3)
                        ++count;
                }
            }
        }
    }
    return count;
}
