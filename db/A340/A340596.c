/*
 * A340596  Number of co-balanced factorizations of n.
 * https://oeis.org/A340596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into exactly
 *            A001221(n) (= the number of distinct prime divisors of n) parts
 *            > 1
 * Range:     u8 n <= 9540, u16 n <= 9540, u32 n <= 9540, u64 n <= 9540,
 *            u128 n <= 9540, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helper: count unordered factorizations of 'rem' into 'parts' factors,
   each factor ≥ 'start' to avoid permutations                           */
static seq_t dfs(idx_t rem, idx_t start, idx_t parts)
{
    if (parts == 1) {
        /* last factor must be at least start                                             */
        return (rem >= start) ? (seq_t)1 : (seq_t)0;
    }
    seq_t cnt = 0;
    /* try all possible factors f ≥ start, f*f ≤ rem                                 */
    for (idx_t f = start; f <= rem / f; ++f) {
        if (rem % f == 0) {
            cnt += dfs(rem / f, f, parts - 1);
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 1) {                 /* 1 has 0 distinct prime factors → empty factorization */
        return (seq_t)1;
    }

    /* compute ω(n) = number of distinct prime factors of n                      */
    idx_t tmp = n;
    idx_t omega = 0;

    if ((tmp & 1) == 0) {         /* handle factor 2 separately                       */
        omega++;
        while ((tmp & 1) == 0) tmp >>= 1;
    }

    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            omega++;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {                 /* remaining prime factor                          */
        omega++;
    }

    /* count unordered factorizations into exactly 'omega' parts                 */
    return dfs(n, 2, omega);
}
