/*
 * A387016  Permutation of the odd integers >= 3 formed by ordering them first
 *          by odd k >= 3 and then by integer m >= 1 in their unique
 *          representation (k - 2^m)*2^m + 1.
 * https://oeis.org/A387016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   generate the permutation of odd integers ≥ 3 as a(n) = (k -
 *            2^m)·2^m + 1, where k runs over odd integers ≥ 3 in increasing
 *            order and, for each k, m runs over 1 … floor(log2 k)
 * Range:     u8 n <= 52, u16 n <= 1800, u32 n <= 9912, u64 n <= 9912,
 *            u128 n <= 9912, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) returns the n-th term (1‑based) of A387016. */
seq_t a(idx_t n)
{
    idx_t count = 0;                    /* how many terms generated so far */

    for (idx_t k = 3; ; k += 2) {      /* odd k = 3,5,7,… */
        /* floor(log2(k)) – number of m values for this k */
        unsigned mmax = 0;
        idx_t tmp = k;
        while (tmp > 1) {               /* shift until only highest bit remains */
            tmp >>= 1;
            ++mmax;
        }

        seq_t pow2m = 1;                /* 2^(m-1), will become 2^m at loop start */
        for (unsigned m = 1; m <= mmax; ++m) {
            pow2m <<= 1;                /* pow2m = 2^m */

            seq_t t = (((seq_t)k - pow2m) * pow2m) + 1; /* term formula */

            ++count;
            if (count == n)               /* found the n‑th term */
                return t;
        }
    }
}
