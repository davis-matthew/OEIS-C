/*
 * A243912  Numbers n > 1 such that n^k never ends in 2 or more identical
 *          digits for any k.
 * https://oeis.org/A243912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the nth term of A243912: numbers n>1 such that n^k never ends in two or more identical
   decimal digits, equivalently n ≡ 1,3,5,6,7,9,15,16,18,21,23,24,25,26,27,29,32,35,36,41,43,45,46,47,49,51,56,57,61,63,65,67,68,69,74,75,76,81,82,83,85,86,87,89,93,95,96 (mod 100). */

#include <stdbool.h>
#include <stdint.h>

static const unsigned char good[100] = {
    [0] = 0, [1] = 1, [2] = 0, [3] = 1, [4] = 0, [5] = 1, [6] = 1,
    [7] = 1, [8] = 0, [9] = 1, [10] = 0, [11] = 0, [12] = 0, [13] = 0,
    [14] = 0, [15] = 1, [16] = 1, [17] = 0, [18] = 1, [19] = 0, [20] = 0,
    [21] = 1, [22] = 0, [23] = 1, [24] = 1, [25] = 1, [26] = 1, [27] = 1,
    [28] = 0, [29] = 1, [30] = 0, [31] = 0, [32] = 1, [33] = 0, [34] = 0,
    [35] = 1, [36] = 1, [37] = 0, [38] = 0, [39] = 0, [40] = 0, [41] = 1,
    [42] = 0, [43] = 1, [44] = 0, [45] = 1, [46] = 1, [47] = 1, [48] = 0,
    [49] = 1, [50] = 0, [51] = 1, [52] = 0, [53] = 0, [54] = 0, [55] = 0,
    [56] = 1, [57] = 1, [58] = 0, [59] = 0, [60] = 0, [61] = 1, [62] = 0,
    [63] = 1, [64] = 0, [65] = 1, [66] = 0, [67] = 1, [68] = 1, [69] = 1,
    [70] = 0, [71] = 0, [72] = 0, [73] = 0, [74] = 1, [75] = 1, [76] = 1,
    [77] = 0, [78] = 0, [79] = 0, [80] = 0, [81] = 1, [82] = 1, [83] = 1,
    [84] = 0, [85] = 1, [86] = 1, [87] = 1, [88] = 0, [89] = 1, [90] = 0,
    [91] = 0, [92] = 0, [93] = 1, [94] = 0, [95] = 1, [96] = 1, [97] = 0,
    [98] = 0, [99] = 0
};

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t x = 2;                 /* start from 2, as sequence excludes 1 */
    for (;; ++x) {
        if (good[x % 100]) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
