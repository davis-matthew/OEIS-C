/*
 * A341021  Number of partitions of n into 4 distinct nonzero triangular
 *          numbers.
 * https://oeis.org/A341021
 *
 * Offset:    20    (a(20) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 distinct nonzero
 *            triangular numbers
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 20; for n < 20 we return 0 arbitrarily. */
    if (n < 20)
        return 0;

    /* Generate all triangular numbers t_k = k(k+1)/2 that are <= n. */
    /* There are at most about sqrt(2n) such numbers; 2000 is ample for n up to ~2e6. */
    seq_t triangles[2000];
    idx_t tri_size = 0;

    for (idx_t k = 1; ; ++k) {
        seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;
        if (t > n)
            break;
        triangles[tri_size++] = t;
        /* Guard against accidental overflow of the static array. */
        if (tri_size == 2000)
            break;
    }

    seq_t count = 0;

    /* Count quadruples of distinct indices i < j < k < l with */
    /* triangles[i] + triangles[j] + triangles[k] + triangles[l] == n. */
    for (idx_t i = 0; i + 3 < tri_size; ++i) {
        for (idx_t j = i + 1; j + 2 < tri_size; ++j) {

            seq_t sum2 = triangles[i] + triangles[j];
            if (sum2 > n)
                continue;  /* remaining sum is negative */

            for (idx_t k = j + 1; k + 1 < tri_size; ++k) {
                seq_t sum3 = sum2 + triangles[k];
                if (sum3 > n)
                    continue;

                seq_t remaining = n - sum3;  /* guaranteed non‑negative */

                /* Binary search for the needed fourth triangular number
                   among the indices greater than k. */
                idx_t low = k + 1;
                idx_t high = tri_size;
                while (low < high) {
                    idx_t mid = (low + high) >> 1;
                    if (triangles[mid] < remaining)
                        low = mid + 1;
                    else
                        high = mid;
                }

                if (low < tri_size && triangles[low] == remaining)
                    ++count;
            }
        }
    }

    return count;
}
