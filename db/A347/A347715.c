/*
 * A347715  Number of compositions (ordered partitions) of n into at most 3
 *          cubes.
 * https://oeis.org/A347715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered sums of at most 3 positive cubes equal
 *            to n
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* largest integer t with t^3 <= n */
    idx_t r = 1;
    while ((r + 1) * (r + 1) * (r + 1) <= n) r++;

    /* store all cubes up to r */
    seq_t cube[30];
    for (idx_t i = 1; i <= r; ++i) {
        cube[i] = (seq_t)i * i * i;
    }

    seq_t ans = 0;

    /* one cube */
    for (idx_t i = 1; i <= r; ++i) {
        if (cube[i] == (seq_t)n) {
            ++ans;
        }
    }

    /* ordered pairs of cubes */
    for (idx_t i = 1; i <= r; ++i) {
        seq_t a_cube = cube[i];
        if (a_cube > n) break;
        for (idx_t j = 1; j <= r; ++j) {
            seq_t sum = a_cube + cube[j];
            if (sum == n) {
                ++ans;
            } else if (sum > n) {
                break;
            }
        }
    }

    /* ordered triples of cubes */
    for (idx_t i = 1; i <= r; ++i) {
        seq_t a_cube = cube[i];
        if (a_cube > n) break;
        for (idx_t j = 1; j <= r; ++j) {
            seq_t ab = a_cube + cube[j];
            if (ab > n) break;
            for (idx_t k = 1; k <= r; ++k) {
                seq_t sum = ab + cube[k];
                if (sum == n) {
                    ++ans;
                } else if (sum > n) {
                    break;
                }
            }
        }
    }

    return ans;
}
