/*
 * A329322  a(n) is the total number of even parts in all partitions of n into
 *          consecutive parts.
 * https://oeis.org/A329322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the total number of even parts in all partitions of n
 *            into consecutive parts. The partitions are of the form n = a +
 *            (a+1) + … + (a+ℓ‑1). For each possible ℓ that yields a positive
 *            integer starting value a, count the even numbers in that
 *            partition and add them to the answer
 * Range:     u8 n <= 901, u16 n <= 9381, u32 n <= 9381, u64 n <= 9381,
 *            u128 n <= 9381, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t res = 0;
    seq_t m = (seq_t)n * 2;          /* 2n */
    
    /* Determine the maximum possible length ℓ such that 1+2+…+ℓ ≤ n. */
    idx_t limit = 0;
    while (((seq_t)limit * (limit + 1)) / 2 <= (seq_t)n)
        ++limit;                    /* limit is first ℓ with sum > n */

    for (idx_t ell = 1; ell < limit; ++ell) {
        seq_t ell_s = (seq_t)ell;
        if (m % ell_s != 0)
            continue;               /* 2n must be divisible by ℓ */

        /* 2a = (2n)/ℓ – (ℓ – 1)  must be positive and even. */
        seq_t t = m / ell_s - (ell - 1);
        if (t <= 0 || (t & 1))
            continue;

        seq_t a_start = t >> 1;     /* a = t/2 */

        /* Count even parts in this partition. */
        seq_t evens;
        if ((ell & 1) == 0)          /* ℓ even */
            evens = ell / 2;
        else if ((a_start & 1) == 0) /* ℓ odd, a even */
            evens = (ell + 1) / 2;
        else                         /* ℓ odd, a odd */
            evens = (ell - 1) / 2;

        res += evens;
    }
    return res;
}
