/*
 * A336813  Fixed points of A187769.
 * https://oeis.org/A336813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th m >= 0 with A187769(m) = m. Computed row by row: for
 *            m = 2^r + x with 0 <= x < 2^r and t = popcount(x), A187769(m) =
 *            m <=> S_t + rank_t(x) = x, where S_t = Sum_{j<t} C(r,j) and
 *            rank_t(x) is x's position among the r-bit values with t ones.
 *            For each t, S_t + k - x_k does not increase wi...
 * Range:     u8 n <= 46, u16 n <= 118, u32 n <= 270, u64 n <= 270,
 *            u128 n <= 270, big any n
 * Validated: standard, pass, 76 terms through n = 270 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A336813_pas = 0;      /* Pascal triangle, row i at i*(i+1)/2 */
static idx_t A336813_pas_rows = 0;

static seq_t *A336813_terms = 0;
static idx_t A336813_count = 0;
static idx_t A336813_cap = 0;
static idx_t A336813_next_row = 0;  /* next binary length r to process */

static seq_t A336813_C(idx_t i, idx_t j)
{
    if (j > i) return (seq_t)0;
    return A336813_pas[i * (i + 1) / 2 + j];
}

static void A336813_ensure_pascal(idx_t rows)
{
    if (rows <= A336813_pas_rows) return;
    idx_t total = rows * (rows + 1) / 2;
    if (A336813_pas == 0) A336813_pas = mem_new(seq_t, total);
    else A336813_pas = mem_resize(seq_t, A336813_pas, total);
    for (idx_t i = A336813_pas_rows; i < rows; ++i) {
        idx_t base = i * (i + 1) / 2;
        A336813_pas[base] = (seq_t)1;
        A336813_pas[base + i] = (seq_t)1;
        for (idx_t j = 1; j < i; ++j) {
            idx_t pb = (i - 1) * i / 2;
            A336813_pas[base + j] = A336813_pas[pb + j - 1] + A336813_pas[pb + j];
        }
    }
    A336813_pas_rows = rows;
}

/* k-th (0-based, ascending) r-bit value with exactly t ones */
static seq_t A336813_unrank(idx_t r, idx_t t, seq_t k)
{
    seq_t x = 0;
    idx_t ones = t;
    for (idx_t p = r; p > 0 && ones > 0; --p) {
        idx_t pos = p - 1;
        seq_t cnt0 = A336813_C(pos, ones);
        if (k >= cnt0) {
            x |= ((seq_t)1) << pos;
            k -= cnt0;
            --ones;
        }
    }
    return x;
}

static void A336813_push(seq_t v)
{
    if (A336813_count >= A336813_cap) {
        idx_t nc = A336813_cap ? A336813_cap * 2 : 256;
        if (A336813_terms == 0) A336813_terms = mem_new(seq_t, nc);
        else A336813_terms = mem_resize(seq_t, A336813_terms, nc);
        A336813_cap = nc;
    }
    A336813_terms[A336813_count++] = v;
}

static void A336813_process_row(idx_t r)
{
    A336813_ensure_pascal(r + 1);
    seq_t high = ((seq_t)1) << r;
    seq_t S = 0;
    for (idx_t t = 0; t <= r; ++t) {
        seq_t C = A336813_C(r, t);
        /* smallest k in [0,C) with S + k <= x_k */
        seq_t lo = 0, hi = C;
        while (lo < hi) {
            seq_t mid = lo + (hi - lo) / 2;
            seq_t xm = A336813_unrank(r, t, mid);
            if (S + mid <= xm) hi = mid;
            else lo = mid + 1;
        }
        seq_t k = lo;
        int guard = 0;
        while (k < C && guard < 3) {
            seq_t xk = A336813_unrank(r, t, k);
            if (S + k != xk) break;
            A336813_push(high + xk);
            ++k;
            ++guard;
        }
        S += C;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A336813_count == 0) A336813_push((seq_t)0);   /* m = 0 */
    while (A336813_count < n) {
        A336813_process_row(A336813_next_row);
        ++A336813_next_row;
    }
    return A336813_terms[n - 1];
}
