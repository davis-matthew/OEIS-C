/*
 * A248439  Number of length 6+2 0..n arrays with every three consecutive
 *          terms having the sum of some two elements equal to twice the
 *          third.
 * https://oeis.org/A248439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 24, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computation of sequence A248439:
 * number of length‑8 arrays over 0..n where every consecutive
 * triple has one element equal to the arithmetic mean of the
 * other two.  The algorithm enumerates all 3^6 patterns of
 * difference multipliers (1, –2, –1/2), then for each possible
 * initial difference d₁ counts the number of starting values a₁
 * that keep all terms inside the bounds.  Constant sequences,
 * counted 3^6 times, are corrected for over‑counting.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    const int MODS = 729;          /* 3^6 patterns */
    const int VARS = 6;           /* number of multipliers */
    seq_t total = 0;

    /* Patterns of multipliers: 0 -> *1, 1 -> *(-2), 2 -> *(-1/2) */
    int m[VARS];

    /* n fits in signed int for the given limits (≤ 10000) */
    int n_int = (int)n;
    for (int code = 0; code < MODS; ++code) {
        /* Decode pattern from base‑3 representation */
        int tmp = code;
        for (int i = 0; i < VARS; ++i) {
            m[i] = tmp % 3;
            tmp /= 3;
        }

        /* Enumerate all possible initial differences d1 */
        for (int d1 = -n_int; d1 <= n_int; ++d1) {
            long long d = d1;          /* current difference */
            long long pref[8];         /* prefix sums of differences */
            pref[0] = 0;
            pref[1] = d;
            long long sum = pref[1];
            int ok = 1;

            for (int step = 0; step < VARS; ++step) {
                if (m[step] == 0) {           /* *1   */
                    /* d stays the same */
                } else if (m[step] == 1) {    /* *(-2) */
                    d = -2LL * d;
                } else {                      /* *(-1/2) */
                    if (d & 1LL) {            /* odd, cannot divide by 2 */
                        ok = 0;
                        break;
                    }
                    d = -(d / 2);
                }
                sum += d;
                pref[step + 2] = sum;
            }
            if (!ok) continue;

            /* Determine allowed range for a1 */
            long long lower = -pref[0];              /* 0 */
            long long upper = (long long)n - pref[0];/* n */
            for (int i = 1; i <= 7; ++i) {
                long long l = -pref[i];
                long long u = (long long)n - pref[i];
                if (l > lower) lower = l;
                if (u < upper) upper = u;
                if (upper < lower) break;           /* no possible a1 */
            }
            if (upper >= lower) {
                total += (seq_t)(upper - lower + 1);
            }
        }
    }

    /* Correct over‑counting of constant sequences (d1 = 0) */
    const seq_t OVER = 728;                /* 3^6 - 1 */
    if (total >= (OVER * ((seq_t)n + 1))) {
        total -= OVER * ((seq_t)n + 1);
    } else {
        total = 0;
    }

    return total;
}
