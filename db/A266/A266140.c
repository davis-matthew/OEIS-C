/*
 * A266140  Palindromes such that removing at most one digit will result in a
 *          term in A110784.
 * https://oeis.org/A266140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th number (with 0 first) among palindromes of length L
 *            whose first floor(L/2) digits are nondecreasing (first digit >=
 *            1) and, for odd L, whose middle digit is arbitrary. This is the
 *            union of A266139 and A110784. The code does a combinatorial
 *            unranking by length, then by digits
 * Range:     u8 n <= 35, u16 n <= 504, u32 n <= 8899, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9709 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A266140_C[160][10];
static bool A266140_init = false;

static void A266140_setup(void)
{
    if (A266140_init) return;
    for (int i = 0; i < 160; ++i) {
        A266140_C[i][0] = 1;
        for (int j = 1; j < 10; ++j) {
            if (i == 0) A266140_C[i][j] = 0;
            else A266140_C[i][j] = A266140_C[i-1][j-1] + A266140_C[i-1][j];
        }
    }
    A266140_init = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    A266140_setup();
    uint64_t r = (uint64_t)(n - 1);
    unsigned L = 1;
    unsigned k = 0;
    for (;;) {
        k = L / 2;
        uint64_t cnt = A266140_C[k + 8][8];
        if (L & 1) cnt *= 10;
        if (r < cnt) break;
        r -= cnt;
        ++L;
    }
    int mid = 0;
    if (L & 1) {
        mid = (int)(r % 10);
        r /= 10;
    }
    int digits[80];
    int mind = 1;
    for (unsigned pos = 0; pos < k; ++pos) {
        unsigned rem = k - pos - 1;
        for (int d = mind; d <= 9; ++d) {
            uint64_t c = A266140_C[rem + 9 - d][9 - d];
            if (r < c) {
                digits[pos] = d;
                mind = d;
                break;
            }
            r -= c;
        }
    }
    seq_t val = 0;
    for (unsigned i = 0; i < k; ++i) val = val * 10 + (seq_t)digits[i];
    if (L & 1) val = val * 10 + (seq_t)mid;
    for (int i = (int)k - 1; i >= 0; --i) val = val * 10 + (seq_t)digits[i];
    return val;
}
