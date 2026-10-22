/*
 * A397695  Residue class of the decimal digits of n taken mod 3, with 0
 *          represented by 3, and in nondecreasing order.
 * https://oeis.org/A397695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenate the sorted residues of the decimal digits of
 *            n modulo 3, with remainder 0 represented by 3. Residues are
 *            sorted in nondecreasing order. For example, a(57) = 12 and a(68)
 *            = 23
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;

    /* Count how many digits give residues 1, 2, and 3 */
    unsigned cnt[4] = {0, 0, 0, 0};
    idx_t tmp = n;
    while (tmp > 0)
    {
        unsigned d = tmp % 10;
        unsigned r = d % 3;
        if (r == 0)
            r = 3;
        ++cnt[r];
        tmp /= 10;
    }

    /* Build the result by appending the residues in sorted order */
    seq_t res = 0;
    for (unsigned r = 1; r <= 3; ++r)
    {
        for (unsigned i = 0; i < cnt[r]; ++i)
        {
            res = res * 10 + r;
        }
    }
    return res;
}
