/*
 * A382183  Binary sequence linking A105774 and A382113.
 * https://oeis.org/A382183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A105774(n) - ceiling(A382113(n-1)/g), g = (1+sqrt(5))/2.
 *            Here ceiling(m/g) = floor((m-1)/g + 1/g) + 1 =
 *            Zeckendorf-shift(m-1) + 1 for m >= 1, and 0 for m = 0
 * Range:     u8 n <= 33, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* ---------- A105774 ---------- */
static seq_t *A105774_fib = NULL;
static size_t A105774_fib_cap = 0;
static size_t A105774_fib_len = 0;
static seq_t *A105774_memo = NULL;
static size_t A105774_memo_cap = 0;

static void A105774_ensure_fib(idx_t target_n)
{
    if (!A105774_fib) {
        A105774_fib_cap = 16;
        A105774_fib = mem_new(seq_t, A105774_fib_cap);
        if (!A105774_fib) exit(EXIT_FAILURE);
        A105774_fib[0] = 0;
        A105774_fib[1] = 1;
        A105774_fib[2] = 1;
        A105774_fib_len = 3;
    }
    while (A105774_fib_len <= target_n + 1 || A105774_fib[A105774_fib_len-1] <= (seq_t)target_n) {
        if (A105774_fib_len >= A105774_fib_cap) {
            A105774_fib_cap <<= 1;
            seq_t *p = mem_resize(seq_t, A105774_fib, A105774_fib_cap);
            if (!p) { mem_free(A105774_fib); exit(EXIT_FAILURE); }
            A105774_fib = p;
        }
        A105774_fib[A105774_fib_len] = A105774_fib[A105774_fib_len-1] + A105774_fib[A105774_fib_len-2];
        A105774_fib_len++;
    }
}

static void A105774_ensure_memo(idx_t n)
{
    if (!A105774_memo || A105774_memo_cap <= n) {
        size_t new_cap = A105774_memo_cap ? A105774_memo_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *p = mem_resize(seq_t, A105774_memo, new_cap);
        if (!p) { mem_free(A105774_memo); exit(EXIT_FAILURE); }
        if (!A105774_memo) {
            for (size_t i = 0; i < new_cap; ++i) p[i] = 0;
        } else {
            for (size_t i = A105774_memo_cap; i < new_cap; ++i) p[i] = 0;
        }
        A105774_memo = p;
        A105774_memo_cap = new_cap;
    }
}

static seq_t A105774_rec(idx_t n)
{
    if (n == 1) return 1;
    A105774_ensure_memo(n);
    if (A105774_memo[n] != 0) return A105774_memo[n];
    A105774_ensure_fib(n);
    idx_t low = 1;
    idx_t high = A105774_fib_len - 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (A105774_fib[mid] < (seq_t)n) low = mid;
        else high = mid;
    }
    idx_t m = low;
    idx_t offset = n - (idx_t)A105774_fib[m];
    seq_t val = A105774_fib[m + 1] - A105774_rec(offset);
    A105774_memo[n] = val;
    return val;
}

static seq_t A105774_a(idx_t n)
{
    if (n == 0) return 0;
    return A105774_rec(n);
}

/* ---------- A382113 ---------- */
static seq_t A382113_a(idx_t n)
{
    static int init = 0;
    static idx_t fib[100];
    static int fib_cnt = 0;
    if (!init) {
        fib[0] = 1;
        fib[1] = 2;
        fib_cnt = 2;
        while (1) {
            idx_t a = fib[fib_cnt - 1];
            idx_t b = fib[fib_cnt - 2];
            if (UINT64_MAX - a < b) break;
            fib[fib_cnt++] = a + b;
            if (fib_cnt >= 100) break;
        }
        init = 1;
    }
    if (n == 0U) return 0;
    int lo = 0, hi = fib_cnt - 1, hiIdx = -1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        if (fib[mid] <= n) { hiIdx = mid; lo = mid + 1; }
        else hi = mid - 1;
    }
    seq_t result = 0;
    uint8_t prefix = 0;
    for (int j = hiIdx; j >= 0; --j) {
        idx_t f = fib[j];
        uint8_t bit = (n >= f) ? 1 : 0;
        if (bit) n -= f;
        prefix ^= bit;
        if (prefix) result += (seq_t)f;
    }
    return result;
}

/* floor((x+1)/phi) via Zeckendorf shift; returns sum of F_{k-1} over Zeckendorf terms F_k of x */
static seq_t shift_zeck(seq_t x)
{
    seq_t *f = mem_new(seq_t, 400);
    if (!f) exit(EXIT_FAILURE);
    int cnt = 1;
    f[0] = 1;
    if (x >= 2) {
        f[1] = 2;
        cnt = 2;
        while (cnt < 399) {
            seq_t p = f[cnt - 1], q = f[cnt - 2];
            if (p > x - q) break;
            f[cnt] = p + q;
            cnt++;
        }
    }
    seq_t result = 0;
    for (int j = cnt - 1; j >= 0; --j) {
        if (f[j] <= x) {
            x -= f[j];
            result += (j == 0) ? (seq_t)1 : f[j - 1];
        }
    }
    mem_free(f);
    return result;
}

seq_t a(idx_t n)
{
    seq_t m = A382113_a(n - 1);
    seq_t c = 0;
    if (m > 0) c = shift_zeck(m - 1) + 1;
    return A105774_a(n) - c;
}
