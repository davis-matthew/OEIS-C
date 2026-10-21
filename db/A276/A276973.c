/*
 * A276973  Sums, in cents, of pairs of United States coins of different
 *          denominations.
 * https://oeis.org/A276973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = nth element of the sorted list of sums of all pairs of
 *            distinct US * coin values {1,5,10,25,50,100} (in cents). If n
 *            exceeds the number of such sums, * a(n) is defined to be 0
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    const idx_t denom_vals[6] = {1, 5, 10, 25, 50, 100};
    static seq_t sums[15];
    static bool initialized = false;

    if (!initialized) {
        idx_t k = 0;
        for (idx_t i = 0; i < 6; ++i) {
            for (idx_t j = i + 1; j < 6; ++j) {
                sums[k++] = (seq_t)denom_vals[i] + (seq_t)denom_vals[j];
            }
        }
        /* insertion sort the 15 sums */
        for (idx_t i = 1; i < 15; ++i) {
            seq_t key = sums[i];
            idx_t j = i;
            while (j > 0 && sums[j - 1] > key) {
                sums[j] = sums[j - 1];
                --j;
            }
            sums[j] = key;
        }
        initialized = true;
    }

    if (n == 0) return 0;
    if (n <= 15) {
        return sums[n - 1];
    } else {
        return 0;
    }
}
