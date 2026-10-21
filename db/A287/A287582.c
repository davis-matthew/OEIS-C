/*
 * A287582  Number of set partitions of [n] such that all absolute differences
 *          between least elements of consecutive blocks and between
 *          consecutive elements within the blocks are not larger than three.
 * https://oeis.org/A287582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 2*a(n-3) - 10*a(n-4) + 11*a(n-5) +
 *            10*a(n-6) - 6*a(n-7) - 7*a(n-8) + a(n-9) + 2*a(n-10),
 *            a(0)=1,a(1)=1,a(2)=2,a(3)=5,a(4)=15,a(5)=46,a(6)=139,a(7)=410,a(8)=1189,a(9)=3397
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache: buf[i] holds a(i) for i < computed */
    static seq_t *buf = NULL;
    static size_t allocated = 0;          /* allocated capacity */
    static size_t computed = 0;           /* number of values already computed (highest index+1) */

    /* initial values for n = 0..9 */
    static const seq_t init[10] = {
        1, 1, 2, 5, 15, 46, 139, 410, 1189, 3397
    };

    /* ensure buffer large enough */
    size_t needed = (size_t)n + 1;
    if (needed > allocated) {
        size_t new_cap = allocated ? allocated : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure – unlikely in tests */
        buf = tmp;
        allocated = new_cap;
    }

    /* initialize first 10 terms if not already done */
    if (computed == 0) {
        size_t init_len = sizeof(init) / sizeof(init[0]);
        for (size_t i = 0; i < init_len && i < allocated; ++i)
            buf[i] = init[i];
        computed = init_len;
    }

    /* compute needed terms up to n */
    if (needed > computed) {
        for (size_t i = computed; i < needed; ++i) {
            if (i < 10) {
                /* already initialized */
                continue;
            }
            /* recurrence: a(n) = 4*a(n-1) - 2*a(n-2) - 2*a(n-3) - 10*a(n-4) 
               + 11*a(n-5) + 10*a(n-6) - 6*a(n-7) - 7*a(n-8) + a(n-9) + 2*a(n-10) */
            buf[i] = 4 * buf[i - 1];
            buf[i] -= 2 * buf[i - 2];
            buf[i] -= 2 * buf[i - 3];
            buf[i] -= 10 * buf[i - 4];
            buf[i] += 11 * buf[i - 5];
            buf[i] += 10 * buf[i - 6];
            buf[i] -= 6 * buf[i - 7];
            buf[i] -= 7 * buf[i - 8];
            buf[i] += buf[i - 9];
            buf[i] += 2 * buf[i - 10];
        }
        computed = needed;
    }

    return buf[n];
}
