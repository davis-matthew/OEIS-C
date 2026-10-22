/*
 * A369366  Least number of inequivalent cells in a polyiamond with n cells.
 * https://oeis.org/A369366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is given by the following piecewise expressions based on n
 *            mod 6 (n ≥ 1) r = n % 6 r = 0 : a(n) = 1 if n == 6, otherwise 2
 *            r = 1 : a(1) = 1; for n > 1, a(n) = (n-1)/6 + 2 r = 2 : a(n) =
 *            (n+5)/4 r = 3 : a(n) = (n+9)/6 r = 4 : a(n) = (n+9)/6 r = 5 :
 *            a(n) = 3 * ((n+1)/6)
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % (idx_t)6;
    idx_t res;

    switch (r)
    {
        case 0:
            /* n ≡ 0 (mod 6) */
            if (n == 6)
                res = 1;
            else
                res = 2;
            break;

        case 1:
            /* n ≡ 1 (mod 6) */
            if (n == 1)
                res = 1;
            else
                res = ((n - 1) / (idx_t)6) + (idx_t)2;
            break;

        case 2:
            /* n ≡ 2 (mod 6) */
            res = (n + (idx_t)5) / (idx_t)4;
            break;

        case 3:
        case 4:
            /* n ≡ 3 or 4 (mod 6) */
            res = (n + (idx_t)9) / (idx_t)6;
            break;

        case 5:
            /* n ≡ 5 (mod 6) */
            res = (idx_t)3 * ((n + (idx_t)1) / (idx_t)6);
            break;

        default:
            /* Never reached because r ∈ {0,…,5} */
            res = 0;
            break;
    }

    return (seq_t)res;
}
