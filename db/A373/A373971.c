/*
 * A373971  a(n) = 1 if n can be represented as a sum of 2 distinct positive
 *          cubes, otherwise 0.
 * https://oeis.org/A373971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n can be represented as a sum of two distinct *
 *            positive cubes, else 0. a(0) = 0, a(1) = 0, ... * The program
 *            simply checks all possible pairs of cubes up to the * integer
 *            cube root of n
 * Range:     u8 n <= 3255, u16 n <= 9877, u32 n <= 9877, u64 n <= 9877,
 *            u128 n <= 9877, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;          /* cast to seq_t for safety */

    /* Find integer cube root floor: largest i with i^3 <= n */
    idx_t max_i;
    for (idx_t i = 1;; ++i) {
        seq_t cube = (seq_t)i * i * i;
        if (cube > n_seq) {
            max_i = i - 1;
            break;
        }
    }

    /* Check all unordered pairs (i, j) with i < j <= max_i */
    for (idx_t i = 1; i <= max_i; ++i) {
        seq_t cube_i = (seq_t)i * i * i;
        for (idx_t j = i + 1; j <= max_i; ++j) {
            seq_t cube_j = (seq_t)j * j * j;
            seq_t sum = cube_i + cube_j;
            if (sum == n_seq) {
                return (seq_t)1;
            }
            if (sum > n_seq) {
                break;   /* further j will only increase the sum */
            }
        }
    }
    return (seq_t)0;
}
