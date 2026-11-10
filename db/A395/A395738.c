/*
 * A395738  Index of first occurrence of n in A057853.
 * https://oeis.org/A395738
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = smallest m with A057853(m) = n, found by a search over
 *            3-digit groups; a(3) = 0 ("nul"). A reachability table over the
 *            groups gives the smallest such m. Convention fitted to the
 *            listed terms: "unu" is not written before "mil" (1000 = "mil",
 *            21000 = "dudek mil")
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 44 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const unsigned char A395738_DIGIT_LEN[10]   = {3,3,2,3,4,4,3,3,2,3};
static const unsigned char A395738_TENS_LEN[10]    = {0,3,5,6,7,7,6,6,5,6};
static const unsigned char A395738_HUNDRED_LEN[10] = {0,4,6,7,8,8,7,7,6,7};
static const unsigned char A395738_SINGLE_SCALE[7] = {0,3,7,8,8,11,11};
static const unsigned char A395738_PLURAL_SCALE[7] = {0,3,8,9,9,12,12};

static int A395738_block_len(int val) {
    int len = 0;
    int h = val / 100;
    if (h > 0) len += A395738_HUNDRED_LEN[h];
    int r = val % 100;
    if (r < 10) {
        if (r > 0 || h == 0) len += A395738_DIGIT_LEN[r];
    } else if (r < 20) {
        len += A395738_TENS_LEN[1];
        if (r > 10) len += A395738_DIGIT_LEN[r - 10];
    } else {
        len += A395738_TENS_LEN[r / 10];
        if (r % 10) len += A395738_DIGIT_LEN[r % 10];
    }
    return len;
}

/* letters contributed by group value v at group index k */
static int A395738_f(int k, int v) {
    if (v == 0) return 0;
    int len = A395738_block_len(v);
    if (k == 0) return len;
    if (k == 1) {
        /* "unu" is not written before "mil" */
        if (v % 10 == 1) len -= A395738_DIGIT_LEN[1];
        return len + 3;
    }
    return (v == 1) ? A395738_SINGLE_SCALE[k] : len + A395738_PLURAL_SCALE[k];
}

#define A395738_MAXL 256

seq_t a(idx_t n) {
    static bool init = false;
    static unsigned char fl[7][1000];
    static bool reach[8][A395738_MAXL];
    if (!init) {
        for (int k = 0; k < 7; k++)
            for (int v = 0; v < 1000; v++)
                fl[k][v] = (unsigned char)A395738_f(k, v);
        for (int k = 0; k < 8; k++)
            for (int L = 0; L < A395738_MAXL; L++) reach[k][L] = false;
        reach[0][0] = true;
        for (int k = 0; k < 7; k++)
            for (int L = 0; L < A395738_MAXL; L++) {
                if (!reach[k][L]) continue;
                for (int v = 0; v < 1000; v++) {
                    int t = L + fl[k][v];
                    if (t < A395738_MAXL) reach[k + 1][t] = true;
                }
            }
        init = true;
    }
    if (n == 3) return 0;
    if (n >= A395738_MAXL) return 0;
    int N = (int)n;
    for (int K = 0; K < 7; K++) {
        int g[7];
        bool found = false;
        int rem = N;
        for (int v = 1; v < 1000; v++) {
            int f = fl[K][v];
            if (f <= N && reach[K][N - f]) {
                g[K] = v;
                rem = N - f;
                found = true;
                break;
            }
        }
        if (!found) continue;
        for (int k = K - 1; k >= 0; k--) {
            for (int v = 0; v < 1000; v++) {
                int f = fl[k][v];
                if (f <= rem && reach[k][rem - f]) {
                    g[k] = v;
                    rem -= f;
                    break;
                }
            }
        }
        seq_t res = 0;
        for (int k = K; k >= 0; k--) {
            res = res * 1000 + (seq_t)g[k];
        }
        return res;
    }
    return 0;
}
