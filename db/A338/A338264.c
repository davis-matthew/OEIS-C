/*
 * A338264  Triangle read by rows: T(n,k) is the frequency of occurrence of
 *          binary words, under iterations of making the n-bit binary
 *          representation of k palindromic, by binary-subtracting its bit
 *          reversal, 0 <= k < 2^n.
 * https://oeis.org/A338264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) computed from A338264 a(n) is the flattened triangle whose
 *            row m has 2^m terms: T(m,k) = number of times the n‑bit word k
 *            occurs as a non‑initial term in the process: while x is not a
 *            palindrome: x = (x - rev(x)) mod 2^m count occurrence of x
 *            Palindromic start words are not counted
 * Range:     u8 n <= 767, u16 n <= 9329, u32 n <= 9329, u64 n <= 9329,
 *            u128 n <= 9329, big any n
 * Helpers:   ceil_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ceil_log2_uint64.h"

static inline uint64_t rev_bits(uint64_t x, unsigned m)
{
    uint64_t y = 0;
    for (unsigned i = 0; i < m; ++i) {
        y = (y << 1) | (x & 1ULL);
        x >>= 1;
    }
    return y;
}

static inline int is_palindrome(uint64_t x, unsigned m)
{
    return rev_bits(x, m) == x;
}

/* cache of computed rows */
static seq_t *rows[65] = {NULL};
static size_t row_len[65] = {0};

static void compute_row(unsigned m)
{
    if (rows[m] != NULL)
        return;                 /* already computed */
    size_t len = (size_t)1ULL << m;
    seq_t *cnt = (seq_t *)malloc(len * sizeof(seq_t));
    if (!cnt) abort();          /* allocation failure */
    memset(cnt, 0, len * sizeof(seq_t));
    uint64_t mask = len - 1;

    for (uint64_t start = 0; start < len; ++start) {
        if (is_palindrome(start, m))
            continue;           /* starting palindrome: skip */

        uint64_t x = start;
        while (1) {
            x = (x - rev_bits(x, m)) & mask;
            ++cnt[x];
            if (is_palindrome(x, m))
                break;
        }
    }
    rows[m] = cnt;
    row_len[m] = len;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* determine row m such that 2^(m+1)-2 >= n > 2^m-2 */
    uint64_t t = n + 2;                       /* t > 0 for n>=1 */
    int ceil_log = ceil_log2_uint64(t);
    unsigned m = (unsigned)(ceil_log - 1);     /* m >= 1 */

    /* compute row if needed */
    compute_row(m);

    /* offset within row */
    size_t offset = (size_t)(n - ((uint64_t)1 << m) + 1);
    return rows[m][offset];
}
