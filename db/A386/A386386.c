/*
 * A386386  Permutation of the nonnegative integers formed by their unique
 *          representation (k - 2^m)*2^(m-1) - 1, where 1 < 2^m < k odd,
 *          sorted first by k then by m.
 * https://oeis.org/A386386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (k - 2^m) * 2^(m-1) - 1, where k runs over odd integers
 *            >1 and for each k, m runs from 1 to ⌊log₂(k‑1)⌋. The terms are
 *            sorted by k then by m
 * Range:     u8 n <= 91, u16 n <= 2951, u32 n <= 9001, u64 n <= 9001,
 *            u128 n <= 9001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes ⌊log₂(x)⌋ for a non‑negative integer x. (x is at least 1.) */
static idx_t floor_log2_uint64(idx_t x)
{
    idx_t r = 0;
    while (x > 1U) {
        x >>= 1;
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* cumulative number of terms seen so far */
    idx_t cum = 0;
    /* start with the first odd k > 1 */
    idx_t k = 3;

    while (1) {
        /* maximum exponent m such that 2^m < k */
        idx_t mmax = floor_log2_uint64(k - 1U);
        if (cum + mmax > n) {
            /* index n lies in this block */
            idx_t offset = n - cum;          /* 0‑based offset within block */
            idx_t m = offset + 1U;           /* m runs from 1 to mmax */

            /* compute 2^m and 2^(m-1) as seq_t values */
            seq_t two_m   = ((seq_t)1) << m;          /* 2^m */
            seq_t two_m1  = ((seq_t)1) << (m - 1U);   /* 2^(m‑1) */
            seq_t k_seq   = (seq_t)k;                 /* cast k to seq_t */

            /* t = (k - 2^m) * 2^(m-1) - 1 */
            return (k_seq - two_m) * two_m1 - (seq_t)1;
        }
        /* move to the next odd k */
        cum += mmax;
        k += 2;
    }
}
