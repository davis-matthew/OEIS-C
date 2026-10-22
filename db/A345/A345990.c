/*
 * A345990  Smallest m such that n divides m*(m+1)*(m+2).
 * https://oeis.org/A345990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m ≥ 1 such that n divides m·(m+1)·(m+2)
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset (a(1) = 1). For n == 0, return 0 as a safe default. */
    if (n == 0)
        return 0;

    idx_t mod = n;
    /* Candidate for residue 0: the first positive m with m ≡ 0 (mod n) is m = n. */
    seq_t best = (seq_t)n;

    for (idx_t r = 1; r < n; ++r) {
        /* Compute r*(r+1)*(r+2) modulo n using a wider integer type to avoid overflow. */
        unsigned long long prod = (unsigned long long)r * (r + 1) % mod;
        prod = prod * (r + 2) % mod;

        if (prod == 0) {
            seq_t cand = (seq_t)r;       /* r is positive, so m = r. */
            if (cand < best)
                best = cand;
        }
    }

    return best;
}
