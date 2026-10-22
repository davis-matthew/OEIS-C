/*
 * A367588  Number of integer partitions of n with exactly two distinct parts,
 *          both appearing with the same multiplicity.
 * https://oeis.org/A367588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, d>=3} floor((d-1)/2)
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count partitions of n with exactly two distinct parts of equal multiplicity */

/* Note: n is of type idx_t (unsigned 64‑bit).  seq_t is the output type. */
seq_t a(idx_t n)
{
    /* For n < 3 there are no such partitions. */
    if (n < 3)
        return 0;

    seq_t sum = 0;
    /* Enumerate all divisors of n. Use i <= n / i to avoid overflow. */
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)            /* i is a divisor */
        {
            idx_t d1 = i;
            idx_t d2 = n / i;      /* paired divisor */

            if (d1 >= 3)
                sum += (seq_t)(d1 - 1) / 2;
            if (d2 != d1 && d2 >= 3)
                sum += (seq_t)(d2 - 1) / 2;
        }
    }
    return sum;
}
