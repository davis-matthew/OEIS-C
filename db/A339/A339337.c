/*
 * A339337  Dirichlet g.f.: Product_{k>=2} (1 + k^(-s))^5.
 * https://oeis.org/A339337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of n^(-s) in ∏_{k>=2} (1 + k^(-s))^5 It is an
 *            arithmetic function defined by the recurrence below
 * Range:     u8 n <= 23, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    idx_t n;
    idx_t mink;
    seq_t val;
} memo_entry_t;

/* --- simple memoisation table ---------------------------------------- */
#define MEMO_CAP 200000
static memo_entry_t memo[MEMO_CAP];
static size_t memo_cnt = 0;

static bool memo_find(idx_t n, idx_t mink, seq_t *out) {
    for (size_t i = 0; i < memo_cnt; ++i) {
        if (memo[i].n == n && memo[i].mink == mink) {
            *out = memo[i].val;
            return true;
        }
    }
    return false;
}

static void memo_store(idx_t n, idx_t mink, seq_t val) {
    if (memo_cnt < MEMO_CAP) {
        memo[memo_cnt].n = n;
        memo[memo_cnt].mink = mink;
        memo[memo_cnt].val = val;
        ++memo_cnt;
    }
}

/* --- helper: compute k^e but stop (return 0) if result > limit or overflows   */
static inline idx_t pow_leq(idx_t base, int exp, idx_t limit) {
    __int128 r = 1;
    for (int i = 0; i < exp; ++i) {
        r *= base;
        if (r > limit) return 0;      /* overflow or too large */
    }
    return (idx_t)r;
}

/* --- recursive computation ------------------------------------------ */
static seq_t compute_a_rec(idx_t n, idx_t mink) {
    if (n == 1) return 1;
    seq_t cached;
    if (memo_find(n, mink, &cached)) return cached;

    /* coefficients of the binomial expansion of (1 + x)^5 */
    static const seq_t coeff[6] = {0, 5, 10, 10, 5, 1};

    seq_t sum = 0;
    for (int e = 1; e <= 5; ++e) {
        /* lower bound for k to enforce non‑decreasing k in a factorisation  */
        idx_t k_start = (mink > 2) ? mink : 2;
        for (idx_t k = k_start; ; ++k) {
            idx_t power = pow_leq(k, e, n);
            if (power == 0) break;                 /* k^e > n  */
            if (n % power == 0) {
                seq_t sub = compute_a_rec(n / power, k + 1);
                sum += coeff[e] * sub;
            }
        }
    }
    memo_store(n, mink, sum);
    return sum;
}

/* --- public API ------------------------------------------------------- */
seq_t a(idx_t n) {
    return compute_a_rec(n, 2);
}
