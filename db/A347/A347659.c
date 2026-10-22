/*
 * A347659  Number of partitions of n into at most 4 powers of 2.
 * https://oeis.org/A347659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions of n into at most 4 powers
 *            of 2
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* Generate all powers of 2 up to n */
    idx_t pow[65];
    idx_t m = 0;
    idx_t val = 1;
    while (1) {
        pow[m++] = val;
        if (val > n / 2) break;
        val <<= 1;
    }

    seq_t count = 0;
    seq_t N = (seq_t)n;

    for (idx_t i = 0; i < m; ++i) {
        seq_t s1 = (seq_t)pow[i];
        if (s1 == N) ++count;   // one part
        for (idx_t j = i; j < m; ++j) {
            seq_t s2 = s1 + (seq_t)pow[j];
            if (s2 == N) ++count;   // two parts
            for (idx_t k = j; k < m; ++k) {
                seq_t s3 = s2 + (seq_t)pow[k];
                if (s3 == N) ++count;   // three parts
                for (idx_t l = k; l < m; ++l) {
                    seq_t s4 = s3 + (seq_t)pow[l];
                    if (s4 == N) ++count;   // four parts
                }
            }
        }
    }
    return count;
}
