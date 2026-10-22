/*
 * A322184  List of primes that appear in the 3 X 3 matrices A, B, C which are
 *          the smallest solution to the matrix equation AB = C in square
 *          matrices composed of distinct primes.
 * https://oeis.org/A322184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes appearing in the minimal 3×3 matrices A, B, C with AB = C. */
/* The primes are obtained by multiplying the known minimal matrices
 * A and B and collecting all entries of A, B, and C.
 * The set is sorted and a(n) is the n‑th smallest prime (n≥1). */

seq_t a(idx_t n)
{
    static seq_t seq[27];
    static int init = 0;
    if (!init) {
        init = 1;
        /* entries of the known minimal matrices A and B */
        const int A[3][3] = { {3, 29, 19},
                              {7, 17, 11},
                              {5, 13, 23} };
        const int B[3][3] = { {67, 61, 59},
                              {37, 43, 47},
                              {41, 53, 31} };

        /* gather primes from A and B (18 entries) */
        int i, j;
        for (i = 0; i < 3; ++i) {
            seq[i*3 + 0] = (seq_t)A[i][0];
            seq[i*3 + 1] = (seq_t)A[i][1];
            seq[i*3 + 2] = (seq_t)A[i][2];
        }
        for (i = 0; i < 3; ++i) {
            seq[9 + i*3 + 0] = (seq_t)B[i][0];
            seq[9 + i*3 + 1] = (seq_t)B[i][1];
            seq[9 + i*3 + 2] = (seq_t)B[i][2];
        }

        /* compute C = A * B, store its primes (9 entries) */
        for (i = 0; i < 3; ++i) {
            for (j = 0; j < 3; ++j) {
                seq_t sum = (seq_t)A[i][0] * B[0][j] +
                            (seq_t)A[i][1] * B[1][j] +
                            (seq_t)A[i][2] * B[2][j];
                seq[18 + i*3 + j] = sum;
            }
        }

        /* sort all 27 primes in ascending order (simple insertion sort) */
        int idx;
        for (idx = 1; idx < 27; ++idx) {
            seq_t key = seq[idx];
            int pos = idx - 1;
            while (pos >= 0 && seq[pos] > key) {
                seq[pos + 1] = seq[pos];
                --pos;
            }
            seq[pos + 1] = key;
        }
    }
    /* index n starts at 1 */
    if (n == 0) return 0;
    return seq[n - 1];
}
