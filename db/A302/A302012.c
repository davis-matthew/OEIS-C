/*
 * A302012  Number of 5Xn 0..1 arrays with every element equal to 0, 1, 2 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A302012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) for n>15 is defined by the 12‑term linear recurrence a(n) =
 *            24 a(n‑1) + 100 a(n‑2) – 348 a(n‑3) – 1856 a(n‑4) + 1536 a(n‑5)
 *            + 14848 a(n‑6) + 2048 a(n‑7) – 65024 a(n‑8) – 40960 a(n‑9) +
 *            139264 a(n‑10) + 102400 a(n‑11) – 147456 a(n‑12), with the first
 *            15 values: 16, 512, 13616, 368144, 9989376...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation:  iterative application of the recurrence.  */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* first 15 values – all accessed by index.  */
    static seq_t init[16];
    static bool init_done = false;
    if (!init_done) {
        init_done = true;
        init[0] = (seq_t)0;                           /* dummy for 1‑based */
        init[1] = seq_from_str("16");
        init[2] = seq_from_str("512");
        init[3] = seq_from_str("13616");
        init[4] = seq_from_str("368144");
        init[5] = seq_from_str("9989376");
        init[6] = seq_from_str("270990144");
        init[7] = seq_from_str("7350348800");
        init[8] = seq_from_str("199375282176");
        init[9] = seq_from_str("5407964088576");
        init[10] = seq_from_str("146688598302720");
        init[11] = seq_from_str("3978862338503680");
        init[12] = seq_from_str("107924853732951040");
        init[13] = seq_from_str("2927413180587028480");
        init[14] = seq_from_str("79404767608300756992");
        init[15] = seq_from_str("2153818654894959054848");
    }

    if (n <= 15) return init[n];

    /*  Sliding window of the last twelve values:  w[0]=a(n‑1), … ,w[11]=a(n‑12).  */
    seq_t w[12];
    for (size_t i = 0; i < 12; ++i)
        w[i] = init[15 - i];    /* w[0] = a15, w[1] = a14, … , w[11] = a4 */

    for (idx_t cur = 16; cur <= n; ++cur) {
        seq_t sum_pos = w[0]*24 + w[1]*100 + w[4]*1536 + w[5]*14848
                        + w[6]*2048 + w[9]*139264 + w[10]*102400;
        seq_t sum_neg = w[2]*348 + w[3]*1856 + w[7]*65024
                        + w[8]*40960 + w[11]*147456;
        seq_t next = sum_pos - sum_neg;              /* guaranteed non‑negative */

        /* shift window: w[0] becomes next, others shift right.  */
        for (size_t i = 11; i > 0; --i) w[i] = w[i-1];
        w[0] = next;
    }

    return w[0];
}
