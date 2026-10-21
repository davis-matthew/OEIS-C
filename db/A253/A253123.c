/*
 * A253123  Number of length n+2 0..2 arrays with the sum of medians of
 *          adjacent triples multiplied by some arrangement of +-1 equal to
 *          zero.
 * https://oeis.org/A253123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+2 0..2 arrays where there exists a
 *            sign assignment ±1 to the medians of all adjacent triples such
 *            that the signed sum of these medians is zero
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* return the median of three 0..2 values */
static inline int median3(int a, int b, int c)
{
    if ((a <= b && b <= c) || (c <= b && b <= a)) return b;
    if ((b <= a && a <= c) || (c <= a && a <= b)) return a;
    return c;
}

seq_t a(idx_t n)
{
    /* length of the array */
    idx_t L = n + 2;

    /* total number of arrays of length L over {0,1,2} */
    seq_t total = 1;
    for (idx_t i = 0; i < L; ++i) total *= (seq_t)3;

    /* ---------- DP to count arrays with an odd number of 1-medians ---------- */
    seq_t dp_even[9], dp_odd[9];
    seq_t next_even[9], next_odd[9];
    {
        /* initialise for the first two positions (no median yet) */
        for (int i = 0; i < 9; ++i) dp_even[i] = dp_odd[i] = 0;
        for (int a = 0; a < 3; ++a)
            for (int b = 0; b < 3; ++b)
                dp_even[a * 3 + b] += (seq_t)1;
        /* process positions 3 .. L */
        for (idx_t pos = 3; pos <= L; ++pos) {
            for (int i = 0; i < 9; ++i) next_even[i] = next_odd[i] = 0;
            for (int last = 0; last < 9; ++last) {
                int a = last / 3;
                int b = last % 3;
                seq_t cnt_even = dp_even[last];
                seq_t cnt_odd  = dp_odd[last];
                if (!cnt_even && !cnt_odd) continue;
                for (int v = 0; v < 3; ++v) {
                    int med = median3(a, b, v);
                    int nlast = b * 3 + v;
                    if (med == 1) {
                        /* parity flips */
                        next_odd[nlast]  += cnt_even;
                        next_even[nlast] += cnt_odd;
                    } else {
                        /* parity unchanged */
                        next_even[nlast] += cnt_even;
                        next_odd[nlast]  += cnt_odd;
                    }
                }
            }
            /* copy back */
            for (int i = 0; i < 9; ++i) {
                dp_even[i] = next_even[i];
                dp_odd[i]  = next_odd[i];
            }
        }
        /* total arrays with an odd number of 1-medians */
    }

    seq_t arrays_c1_odd = 0;
    for (int i = 0; i < 9; ++i) arrays_c1_odd += dp_odd[i];

    /* ---------- DP to count arrays with no 1-medians and an odd number of 2-medians ---------- */
    static const int STATES = 9 * 4;  /* last pair (9), seen1 (2), parity2 (2) */
    seq_t dp[STATES], next[STATES];

    {
        /* initialise: no 1-median seen, parity of 2-medians is even (0) */
        for (int i = 0; i < STATES; ++i) dp[i] = 0;
        for (int a = 0; a < 3; ++a)
            for (int b = 0; b < 3; ++b) {
                int last = a * 3 + b;
                dp[last * 4 + 0] += (seq_t)1;   /* seen1=0, parity2=0 */
            }

        for (idx_t pos = 3; pos <= L; ++pos) {
            for (int i = 0; i < STATES; ++i) next[i] = 0;
            for (int last = 0; last < 9; ++last) {
                int a = last / 3;
                int b = last % 3;
                for (int seen = 0; seen < 2; ++seen) {
                    for (int p2 = 0; p2 < 2; ++p2) {
                        seq_t cnt = dp[(last * 4) + (seen << 1) + p2];
                        if (!cnt) continue;
                        for (int v = 0; v < 3; ++v) {
                            int med = median3(a, b, v);
                            int nlast = b * 3 + v;
                            int nseen = seen | (med == 1);
                            int np2   = p2 ^ (med == 2);
                            next[(nlast * 4) + (nseen << 1) + np2] += cnt;
                        }
                    }
                }
            }
            /* copy back */
            for (int i = 0; i < STATES; ++i) dp[i] = next[i];
        }
    }

    seq_t arrays_c1zero_c2odd = 0;
    for (int last = 0; last < 9; ++last)
        arrays_c1zero_c2odd += dp[(last * 4) + (0 << 1) + 1];  /* seen1=0, parity2=1 */

    /* valid arrays = total - arrays_c1_odd - arrays_c1_zero_c2_odd */
    seq_t result = total - arrays_c1_odd - arrays_c1zero_c2odd;
    return result;
}
