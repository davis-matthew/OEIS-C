/*
 * A246890  Number of length n+4 0..6 arrays with some pair in every
 *          consecutive five terms totalling exactly 6.
 * https://oeis.org/A246890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 0..6 arrays with some pair in every
 *            consecutive five terms totalling exactly 6. This implementation
 *            uses dynamic programming over the last four elements (there are
 *            7^4 = 2401 possible states)
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 20, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard for safety */

    /* length of the array */
    idx_t L = n + 4;                /* we will generate L elements            */

    /* static tables for state digits and valid next steps */
    static int dig_a[2401], dig_b[2401], dig_c[2401], dig_d[2401];
    static unsigned char count_valid[2401];
    static unsigned char valid_x[2401][7];
    static int initialized = 0;
    if (!initialized) {
        for (int s = 0; s < 2401; ++s) {
            int tmp = s;
            dig_d[s] = tmp % 7; tmp /= 7;
            dig_c[s] = tmp % 7; tmp /= 7;
            dig_b[s] = tmp % 7; tmp /= 7;
            dig_a[s] = tmp % 7;

            count_valid[s] = 0;
            for (int x = 0; x < 7; ++x) {
                int arr[5] = {dig_a[s], dig_b[s], dig_c[s], dig_d[s], x};
                int ok = 0;
                for (int i = 0; i < 5 && !ok; ++i)
                    for (int j = i + 1; j < 5; ++j)
                        if (arr[i] + arr[j] == 6) { ok = 1; break; }
                if (ok) {
                    unsigned char *list = valid_x[s] + count_valid[s];
                    *list = (unsigned char)x;
                    ++count_valid[s];
                }
            }
        }
        initialized = 1;
    }

    /* DP arrays for current and next step */
    static seq_t dp_cur[2401];
    static seq_t dp_next[2401];

    /* Initial state after 4 elements: each state has exactly one sequence */
    for (int i = 0; i < 2401; ++i) dp_cur[i] = 1;

    /* Build the array from element 5 to element L */
    for (idx_t pos = 5; pos <= L; ++pos) {
        /* zero dp_next */
        for (int i = 0; i < 2401; ++i) dp_next[i] = 0;

        for (int s = 0; s < 2401; ++s) {
            seq_t ways = dp_cur[s];
            if (ways == 0) continue;

            int a = dig_a[s], b = dig_b[s], c = dig_c[s], d = dig_d[s];
            unsigned char cnt = count_valid[s];
            unsigned char *list = valid_x[s];

            for (int vi = 0; vi < cnt; ++vi) {
                int x = list[vi];
                int next = (((b * 7) + c) * 7 + d) * 7 + x;
                dp_next[next] += ways;
            }
        }

        /* move to next step */
        for (int i = 0; i < 2401; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all states to obtain the total number of valid arrays */
    seq_t total = 0;
    for (int i = 0; i < 2401; ++i) total += dp_cur[i];

    return total;
}
