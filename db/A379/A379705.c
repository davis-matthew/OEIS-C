/*
 * A379705  a(n) is the least integer k > n such that integers p, q exist for
 *          which n, p, k are in arithmetic and n, q, k are in geometric
 *          progression.
 * https://oeis.org/A379705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least k > n such that (n + k) is even and n divides
 *            q² where k = q² / n for some integer q > n. This is found by
 *            iterating q = n+1, n+2, … until the two conditions hold
 * Range:     u8 n <= 28, u16 n <= 7282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n = 0, harmless */

    for (idx_t q = n + 1; ; ++q) {
        /* Use 64‑bit arithmetic for the intermediate square to avoid overflow
           when seq_t is smaller than 64 bits.  The maximum q for the
           search is less than 4·n, so q² fits comfortably in 64 bits. */
        unsigned long long q64 = (unsigned long long)q;
        unsigned long long q2  = q64 * q64;

        if (q2 % (unsigned long long)n == 0) {          /* n divides q² */
            seq_t k = (seq_t)(q2 / (unsigned long long)n);   /* k > n          */
            if (((seq_t)n + k) % 2 == 0)   /* n + k even      */
                return k;
        }
    }
}
