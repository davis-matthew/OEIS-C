/*
 * A357534  Number of compositions (ordered partitions) of n into two or more
 *          powers of 2.
 * https://oeis.org/A357534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into powers of 2 minus 1 if n
 *            is a power of 2
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n) {
    if (n == 0u) return 0;
    static seq_t *f = NULL;
    static idx_t f_size = 0;
    if (f_size == 0) {
        f = (seq_t*)malloc(sizeof(seq_t));
        if (!f) return 0;
        f[0] = 1;
        f_size = 1;
    }
    if (n + 1 > f_size) {
        idx_t new_size = n + 1;
        seq_t *new_f = (seq_t*)realloc(f, new_size * sizeof(seq_t));
        if (!new_f) return 0;
        f = new_f;
        for (idx_t i = f_size; i <= n; ++i) {
            seq_t sum = 0;
            idx_t pow2 = 1;
            while (pow2 <= i) {
                sum += f[i - pow2];
                pow2 <<= 1;
            }
            f[i] = sum;
        }
        f_size = new_size;
    }
    seq_t res = f[n];
    if (is_power_of_two((seq_t)n))
        res -= 1;
    return res;
}
