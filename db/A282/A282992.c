/*
 * A282992  Number of n X 4 0..1 arrays with no 1 equal to more than one of
 *          its horizontal and vertical neighbors.
 * https://oeis.org/A282992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   linear recurrence of order 15 a(n) = 4*a(n-1)
 *            +27*a(n-2)+59*a(n-3)-10*a(n-4)-134*a(n-5)+12*a(n-6)
 *            +43*a(n-7)-55*a(n-8)+42*a(n-9)-90*a(n-10)+58*a(n-11)-30*a(n-12)
 *            +6*a(n-13)-2*a(n-14)+a(n-15) initial values a(1) … a(15) are
 *            given below
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t buf_len = 0;
    static idx_t max_comp = 0;
    static bool init = false;

    if (!init) {
        buf_len = 15;
        buf = (seq_t *)malloc(buf_len * sizeof(seq_t));
        if (!buf) return 0;
        buf[0]  = (seq_t)13ULL;
        buf[1]  = (seq_t)98ULL;
        buf[2]  = (seq_t)803ULL;
        buf[3]  = (seq_t)6547ULL;
        buf[4]  = (seq_t)53389ULL;
        buf[5]  = (seq_t)435027ULL;
        buf[6]  = (seq_t)3546870ULL;
        buf[7]  = (seq_t)28911809ULL;
        buf[8]  = (seq_t)235681253ULL;
        buf[9]  = (seq_t)1921212987ULL;
        buf[10] = (seq_t)15661161199ULL;
        buf[11] = (seq_t)127665372304ULL;
        buf[12] = (seq_t)1040691953095ULL;
        buf[13] = (seq_t)8483425185009ULL;
        buf[14] = (seq_t)69154476414585ULL;
        max_comp = 15;
        init = true;
    }

    if (n <= max_comp) {
        return buf[n - 1];
    }

    if (n > buf_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(buf, new_len * sizeof(seq_t));
        if (!tmp) return 0;
        buf = tmp;
        buf_len = new_len;
    }

    for (idx_t i = max_comp + 1; i <= n; ++i) {
        idx_t idx = i - 1;               /* zero‑based index in buf */
        seq_t val = 4 * buf[idx - 1]  + 27 * buf[idx - 2] + 59 * buf[idx - 3]
                    - 10 * buf[idx - 4] - 134 * buf[idx - 5] + 12 * buf[idx - 6]
                    + 43 * buf[idx - 7] - 55 * buf[idx - 8] + 42 * buf[idx - 9]
                    - 90 * buf[idx -10] + 58 * buf[idx -11] - 30 * buf[idx -12]
                    + 6  * buf[idx -13] - 2  * buf[idx -14] + buf[idx -15];
        buf[idx] = val;
    }
    max_comp = n;
    return buf[n - 1];
}
