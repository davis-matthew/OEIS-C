/*
 * A294741  Numbers that are the sum of 5 nonzero squares in exactly 7 ways.
 * https://oeis.org/A294741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for n ≤ 5408 enumerate all unordered 5‑tuples of positive
 * integers (a,b,c,d,e) with a²+b²+c²+d²+e² = n.
 * Count the number of distinct multisets of squares {a²,b²,c²,d²,e²}
 * (order is irrelevant).  Numbers that appear in exactly 7 such
 * multisets are stored in increasing order; a(n) returns the nth
 * number in this list.  The search space is only about 9.6 million
 * combinations, which runs easily within the time limit. */

#include <stdbool.h>
#include <stdint.h>

static bool _a294741_init = false;
static seq_t _a294741_terms[5409];
static idx_t _a294741_len = 0;

static void _a294741_precompute(void) {
    const idx_t MAX = 5408;
    /* Counts of unordered solutions for each n. */
    int cnt[5409] = {0};

    /* Squares of positive integers up to 73 (since 73² = 5329 ≤ 5408). */
    int sq[73];
    for (int i = 1; i <= 73; ++i) {
        sq[i - 1] = i * i;
    }
    const int NUM = 73;

    /* Enumerate unordered 5‑tuples (i ≤ j ≤ k ≤ l ≤ m). */
    for (int i = 0; i < NUM; ++i) {
        int a = sq[i];
        if (a * 5 > MAX) break;                     /* minimal sum too large */
        for (int j = i; j < NUM; ++j) {
            int b = sq[j];
            int sum2 = a + b;
            if (sum2 + 4 * b > MAX) break;           /* minimal remaining sum too large */
            for (int k = j; k < NUM; ++k) {
                int c = sq[k];
                int sum3 = sum2 + c;
                if (sum3 + 3 * c > MAX) break;
                for (int l = k; l < NUM; ++l) {
                    int d = sq[l];
                    int sum4 = sum3 + d;
                    if (sum4 + d > MAX) break;
                    for (int m = l; m < NUM; ++m) {
                        int e = sq[m];
                        int sum5 = sum4 + e;
                        if (sum5 > MAX) break;
                        ++cnt[sum5];
                    }
                }
            }
        }
    }

    /* Collect numbers with exactly 7 representations. */
    for (int n = 1; n <= MAX; ++n) {
        if (cnt[n] == 7) {
            _a294741_terms[_a294741_len++] = (seq_t)n;
        }
    }
    _a294741_init = true;
}

seq_t a(idx_t n) {
    if (!_a294741_init) {
        _a294741_precompute();
    }
    if (n == 0 || n > _a294741_len) {
        return (seq_t)0;
    }
    return _a294741_terms[n - 1];
}
