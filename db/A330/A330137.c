/*
 * A330137  Numbers m such that 1 < gcd(m, 30) < m and m does not divide 30^e
 *          for e >= 0.
 * https://oeis.org/A330137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A330137: numbers m containing at least one prime factor 2,3 or 5
   and at least one prime factor >5 (equivalently 1<gcd(m,30)<m and m∤30^e).
   Computed by searching integers that satisfy these conditions. */

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 2; ; ++m)
    {
        /* need at least one small prime factor */
        if ((m % 2 != 0) && (m % 3 != 0) && (m % 5 != 0))
            continue;

        seq_t val = (seq_t)m;
        seq_t tmp = val;

        /* remove all factors 2, 3, 5 */
        while (tmp % 2 == 0) tmp /= 2;
        while (tmp % 3 == 0) tmp /= 3;
        while (tmp % 5 == 0) tmp /= 5;

        /* tmp>1 means m has a prime factor >5 */
        if (tmp > 1)
        {
            ++count;
            if (count == n)
                return val;
        }
    }
}
