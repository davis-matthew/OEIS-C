/*
 * A389683  Number of representations of n as binomial(i,3) + binomial(j,2) +
 *          binomial(k,1) with i,j,k >= 0.
 * https://oeis.org/A389683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(i,j,k) ∈ ℕ⁴ : binomial(i,3) + binomial(j,2) + k = n}|
 *            Enumerates the triples by iterating i and j, computing the
 *            required k as the remaining value. The algorithm is O(√n) per
 *            call
 * Range:     u8 n <= 238, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_seq  = (seq_t)n;

    /* Binning variables for i */
    seq_t ci3 = 0;  /* binomial(i,3) for current i */
    seq_t ci2 = 0;  /* binomial(i,2) for current i */

    for (idx_t i = 0; ; ++i) {
        if (ci3 > n_seq)                    /* further i will be too large */
            break;

        seq_t rem = n_seq - ci3;           /* remaining value to be written as j choose 2 + k */

        /* Iterate over j, using incremental update of binomial(j,2) */
        seq_t cj2 = 0;                     /* binomial(j,2) for current j */
        for (idx_t j = 0; ; ++j) {
            if (cj2 > rem)                     /* no more valid j for this i */
                break;
            ++count;                           /* one solution (k = rem - cj2) */
            cj2 += j;                          /* next binomial(j,2) via recurrence */
        }

        /* Prepare values for next i */
        ci3 += ci2;                      /* C(i+1,3) = C(i,3) + C(i,2) */
        if (i + 1 >= 2)                  /* only when i+1 ≥ 2 is C(i+1,2) defined */
            ci2 += (i + 1) - 1;          /* C(i+1,2) = C(i,2) + i */
    }

    return count;
}
