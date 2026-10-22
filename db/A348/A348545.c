/*
 * A348545  Positive integers with final digit 9 that are equal to the product
 *          of two integers ending with the same digit.
 * https://oeis.org/A348545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer ending in 9 that can be
 *            written as a product of two positive integers both ending with
 *            the same digit. That digit must be 3 or 7 (since 3·3 ≡ 7·7 ≡ 9
 *            (mod 10))
 * Range:     u8 n <= 12, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code searches integers of the form 10k+9 and tests whether there
   exists a divisor d such that d%10==3 and (m/d)%10==3 or
   d%10==7 and (m/d)%10==7. */

seq_t a(idx_t n)
{
    /* candidate numbers to test, all end in 9 */
    seq_t candidate = 9;
    idx_t found = 0;

    for (;;)
    {
        /* test candidate */
        idx_t d;
        int ok = 0;
        for (d = 1; ((seq_t)d * (seq_t)d) <= candidate; ++d)
        {
            seq_t ds = (seq_t)d;
            if (candidate % ds == 0)
            {
                seq_t e = candidate / ds;
                seq_t dm = ds % (seq_t)10;
                seq_t em = e % (seq_t)10;
                if (((dm == 3) && (em == 3)) || ((dm == 7) && (em == 7)))
                {
                    ok = 1;
                    break;
                }
            }
        }

        if (ok)
        {
            ++found;
            if (found == n)
                return candidate;
        }

        candidate += (seq_t)10;   /* next number ending in 9 */
    }
}
