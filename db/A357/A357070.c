/*
 * A357070  Number of partitions of n into at most 2 distinct positive
 *          triangular numbers.
 * https://oeis.org/A357070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of partitions of n into at most 2 distinct positive
 *            triangular numbers
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The empty partition gives a(0) = 1. */
    if (n == 0)
        return (seq_t)1;

    /* Triangular numbers t_k = k(k+1)/2 for k >= 1 are needed only up to n.
       There are at most ~140 of them for n <= 10000, so a small fixed array
       suffices.  idx_t is the build‑specific index type (uint64_t). */
    idx_t tri[200];
    idx_t tri_cnt = 0;

    for (idx_t k = 1;; ++k) {
        idx_t t = k * (k + 1) / 2;
        if (t > n)
            break;
        tri[tri_cnt++] = t;
    }

    seq_t count = 0;

    /* Single triangular part. */
    for (idx_t i = 0; i < tri_cnt; ++i) {
        if (tri[i] == n)
            ++count;
    }

    /* Pairs of distinct triangular parts.  The array is sorted, so we can
       simply check all unordered pairs with i < j.  The count of such pairs
       is always small (≤ tri_cnt^2/2). */
    for (idx_t i = 0; i < tri_cnt; ++i) {
        for (idx_t j = i + 1; j < tri_cnt; ++j) {
            if (tri[i] + tri[j] == n)
                ++count;
        }
    }

    return count;
}
