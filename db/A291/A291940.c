/*
 * A291940  Triangle read by rows: T(n,k) = T(n-k,k-1) - 2*T(n-k,k) +
 *          T(n-k,k+1) with T(0,0) = 1 for 0 <= k <= A003056(n).
 * https://oeis.org/A291940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   T(n,k) = T(n-k,k-1) - 2*T(n-k,k) + T(n-k,k+1), T(0,0)=1,
 *            T(n,0)=0 for n>0, T = 0 outside 0<=k<=A003056(n); a(m) is the
 *            triangle read by rows
 * Range:     i8 n <= 26, i16 n <= 79, i32 n <= 213, i64 n <= 602,
 *            i128 n <= 1700, big any n
 * Validated: standard, pass, 81 terms through n = 9098 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A291940_tab = 0;
static size_t *A291940_start = 0;
static size_t *A291940_wid = 0;
static size_t A291940_rows = 0;

static seq_t A291940_get(size_t r, size_t k)
{
    if (k >= A291940_wid[r]) return (seq_t)0;
    return A291940_tab[A291940_start[r] + k];
}

static void A291940_build(size_t rows)
{
    size_t n, k, s = 0, total = 0;
    if (A291940_tab) { mem_free(A291940_tab); A291940_tab = 0; }
    if (A291940_start) { free(A291940_start); }
    if (A291940_wid) { free(A291940_wid); }
    A291940_start = (size_t *)malloc(rows * sizeof(size_t));
    A291940_wid = (size_t *)malloc(rows * sizeof(size_t));
    for (n = 0; n < rows; n++) {
        while ((s + 1) * (s + 2) / 2 <= n) s++;
        A291940_wid[n] = s + 1;
        A291940_start[n] = total;
        total += s + 1;
    }
    A291940_tab = mem_new(seq_t, total);
    A291940_tab[0] = (seq_t)1;
    for (n = 1; n < rows; n++) {
        size_t w = A291940_wid[n];
        size_t st = A291940_start[n];
        A291940_tab[st] = (seq_t)0;
        for (k = 1; k < w; k++) {
            size_t r = n - k;
            seq_t v = A291940_get(r, k - 1) - 2 * A291940_get(r, k) + A291940_get(r, k + 1);
            A291940_tab[st + k] = v;
        }
    }
    A291940_rows = rows;
}

seq_t a(idx_t n)
{
    size_t row = 0, s = 0;
    uint64_t pos = 0;
    uint64_t m = (uint64_t)n;
    while (pos + (s + 1) <= m) {
        pos += s + 1;
        row++;
        while ((s + 1) * (s + 2) / 2 <= row) s++;
    }
    size_t k = (size_t)(m - pos);
    if (row >= A291940_rows) {
        size_t target = row < 10000 ? 10000 : row + 1;
        A291940_build(target);
    }
    return A291940_tab[A291940_start[row] + k];
}
