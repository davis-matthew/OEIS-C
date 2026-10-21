/*
 * A285830  Triangular array: row n shows the n+1 zero-one subwords of length
 *          n that occur in the infinite Fibonacci word A003849, in the order
 *          of occurrence.
 * https://oeis.org/A285830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) equals the n-th bit obtained by scanning all distinct
 *            binary subwords of the infinite Fibonacci word, arranged
 *            row‑wise; each row r lists the r+1 distinct substrings of length
 *            r in their order of first appearance
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* outside the offset */

    /* ----------------------------------------------------
       1. Find the row r that contains the n-th bit
    ---------------------------------------------------- */
    idx_t r = 1;
    while (((__int128)r * (r + 1) * (r + 2) / 3) < n) ++r;

    __int128 prev_cum = (__int128)(r - 1) * r * (r + 1) / 3;
    idx_t offset = (idx_t)(n - prev_cum - 1);    /* zero‑based inside row r */
    idx_t word_idx   = offset / r;              /* which word          */
    idx_t pos_in_word = offset % r;             /* which bit in word   */

    /* ----------------------------------------------------
       2. Build a prefix of the Fibonacci word long enough
          to contain all distinct subwords of length r
    ---------------------------------------------------- */
    size_t L = 2 * (size_t)r + 3;                 /* safe upper bound */

    /* Build Fibonacci word using the substitution rule
       0 → 01, 1 → 0 until length >= L */
    size_t len0 = 1;
    char *buf0 = (char *)malloc(L);
    buf0[0] = '0';

    size_t len1 = 2;
    char *buf1 = (char *)malloc(L);
    memcpy(buf1, "01", 2);

    char *buf2 = (char *)malloc(L);

    while (len1 < L) {
        size_t p = 0;
        for (size_t i = 0; i < len1 && p < L; ++i) buf2[p++] = buf1[i];
        for (size_t i = 0; i < len0 && p < L; ++i) buf2[p++] = buf0[i];
        size_t len2 = p;

        memcpy(buf0, buf1, len1);
        memcpy(buf1, buf2, len2);
        len0 = len1;
        len1 = len2;
    }

    /* buf1 now holds a prefix of length >= L */
    size_t fib_len = len1;
    if (fib_len > L) fib_len = L;

    /* ----------------------------------------------------
       3. Enumerate the distinct words of length r in order
    ---------------------------------------------------- */
    uint64_t mask = (r >= 64) ? ~0ULL : ((1ULL << r) - 1ULL);

    /* Storage for discovered words – enough for any r <= 63 */
    uint64_t words[64];
    int seen_cnt = 0;

    if (r > 0) {
        /* first window */
        uint64_t val = 0;
        for (size_t i = 0; i < (size_t)r; ++i)
            val = ((val << 1) | (buf1[i] - '0')) & mask;
        words[seen_cnt++] = val;

        /* slide along the prefix, collecting new distinct words */
        for (size_t i = 1; i + r <= fib_len && seen_cnt < r + 1; ++i) {
            val = ((val << 1) | (buf1[i + r - 1] - '0')) & mask;
            bool seen = false;
            for (int j = 0; j < seen_cnt; ++j)
                if (words[j] == val) { seen = true; break; }
            if (!seen) words[seen_cnt++] = val;
        }
    }

    /* ----------------------------------------------------
       4. Return the requested bit
    ---------------------------------------------------- */
    uint64_t target_word = words[word_idx];
    uint64_t bit = (target_word >> (r - 1 - pos_in_word)) & 1ULL;

    /* free dynamically allocated buffers */
    free(buf0);
    free(buf1);
    free(buf2);

    return (seq_t)bit;
}
