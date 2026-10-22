/*
 * A364103  Sum of divisors of 5*n-4 of form 5*k+4.
 * https://oeis.org/A364103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors d of (5*n-4) with d ≡ 4 (mod 5)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.  For n = 0 we
       return 0;  the offset in OEIS starts at 1. */
    if (n == 0)
        return (seq_t)0;

    /* Compute m = 5*n - 4.  All arithmetic is performed in idx_t
       (usually an unsigned 64‑bit type).  This is safe for n up to
       values used in the test harness. */
    idx_t m = 5 * n - 4;

    seq_t sum = 0;

    /* Enumerate all divisors d of m.  We avoid overflow by using
       the condition d ≤ m/d.  For each divisor we add it to the sum
       if it is congruent to 4 modulo 5. */
    for (idx_t d = 1; d <= m / d; ++d)
    {
        if (m % d == 0)
        {
            /* First divisor in the pair */
            if (d % 5 == 4)
                sum += (seq_t)d;

            /* Second divisor in the pair (unless equal to the first) */
            idx_t e = m / d;
            if (e != d && e % 5 == 4)
                sum += (seq_t)e;
        }
    }

    return sum;
}
