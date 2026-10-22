/*
 * A389684  Number of representations of n as binomial(i,4) + binomial(j,3) +
 *          binomial(k,2) + binomial(l,1) with i,j,k,l>=0.
 * https://oeis.org/A389684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(i,j,k,l)≥0 :
 *            binomial(i,4)+binomial(j,3)+binomial(k,2)+l = n}|
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of representations of n as a sum of the binomial
   coefficients C(i,4), C(j,3), C(k,2) and the linear term l = binomial(l,1).  */

static inline seq_t binom4(idx_t i)
{
    if (i < 4) return 0;
    return ((seq_t)i * (i - 1) * (i - 2) * (i - 3)) / 24;
}

static inline seq_t binom3(idx_t j)
{
    if (j < 3) return 0;
    return ((seq_t)j * (j - 1) * (j - 2)) / 6;
}

static inline seq_t binom2(idx_t k)
{
    if (k < 2) return 0;
    return ((seq_t)k * (k - 1)) / 2;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t i = 0;; ++i) {
        seq_t c4 = binom4(i);
        if (c4 > n) break;                    /* larger i will only increase */

        for (idx_t j = 0;; ++j) {
            seq_t c3 = binom3(j);
            seq_t sum_4_3 = c4 + c3;
            if (sum_4_3 > n) break;           /* larger j will only increase */

            for (idx_t k = 0;; ++k) {
                seq_t c2 = binom2(k);
                seq_t sum = sum_4_3 + c2;
                if (sum > n) break;           /* larger k will only increase */

                /* l = n - sum is always a non‑negative integer, so the
                   representation is valid.  Count it. */
                ++count;
            }
        }
    }

    return count;
}
