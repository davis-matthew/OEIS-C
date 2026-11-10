/*
 * A358615  Record high values in A358497.
 * https://oeis.org/A358615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th term of the records of A358497; these are the
 *            numbers 1 followed by a restricted-growth string over digits
 *            2..9 (each digit is at most 1 more than the maximum so far),
 *            listed by length then lexicographically. The code unranks n
 *            directly
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A358615_ROWS 70
#define A358615_CAP ((uint64_t)0x7FFFFFFFFFFFFFFFULL)

static uint64_t A358615_W[A358615_ROWS][10];
static int A358615_init = 0;

static void A358615_setup(void)
{
    int r, k;
    for (k = 0; k <= 8; ++k)
        A358615_W[0][k] = 1;
    for (r = 1; r < A358615_ROWS; ++r) {
        for (k = 0; k <= 8; ++k) {
            uint64_t a = A358615_W[r - 1][k];
            uint64_t v;
            if (k > 0 && a > A358615_CAP / (uint64_t)k)
                v = A358615_CAP;
            else
                v = a * (uint64_t)k;
            if (k < 8) {
                uint64_t b = A358615_W[r - 1][k + 1];
                if (v > A358615_CAP - b)
                    v = A358615_CAP;
                else
                    v += b;
            }
            A358615_W[r][k] = v;
        }
    }
    A358615_init = 1;
}

seq_t a(idx_t n)
{
    if (!A358615_init)
        A358615_setup();

    uint64_t r = (uint64_t)(n - 1);
    int m = 0;
    while (m < A358615_ROWS - 1) {
        uint64_t cnt = A358615_W[m][0];
        if (r < cnt)
            break;
        r -= cnt;
        ++m;
    }

    seq_t result = 1;
    int k = 0;
    for (int pos = m; pos >= 1; --pos) {
        uint64_t c = A358615_W[pos - 1][k];
        int d;
        if (k > 0 && c > 0 && (r / c) < (uint64_t)k) {
            uint64_t idx = r / c;
            r -= idx * c;
            d = (int)idx + 1;
        } else {
            if (k > 0)
                r -= (uint64_t)k * c;
            ++k;
            d = k;
        }
        result = result * 10 + (seq_t)(d + 1);
    }
    return result;
}
