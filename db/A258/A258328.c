/*
 * A258328  L.g.f.: log(1 + Sum_{n>=1} x^(n^2) + x^(3*n^2) ).
 * https://oeis.org/A258328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 71, i16 n <= 1024, i32 n <= 1024, i64 n <= 1024,
 *            i128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n≥1, let S = {m^2, 3m^2 | m≥1}.  Define e(n)=1 if n∈S, otherwise 0. */
/* Then a(n) = n·e(n) – Σ_{k∈S, 1≤k<n} a(n−k). */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- helpers ---------- */

static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

/* ---------- static storage ---------- */

static seq_t *A = NULL;                /* a(n), 0‑based (a[0] unused) */
static size_t A_cap = 0;
static idx_t A_computed = 0;           /* largest n computed so far */

static idx_t *S = NULL;                /* indices of S: m^2 and 3m^2 */
static size_t S_cap = 0;
static size_t S_len = 0;

/* ---------- memory helpers ---------- */

static void ensure_a_capacity(idx_t N)
{
    if (N + 1 > A_cap) {
        size_t new_cap = N + 1;
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        A_cap = new_cap;
        /* initialise the new slot at index 0 */
        if (A != NULL) A[0] = 0;
    }
}

static void ensure_s_capacity(size_t needed)
{
    if (needed > S_cap) {
        S_cap = needed;
        S = (idx_t *)realloc(S, S_cap * sizeof(idx_t));
    }
}

/* ---------- build S up to N ---------- */

static void build_S(idx_t N)
{
    idx_t max_m = floor_sqrt_u64(N);
    size_t required = 2 * max_m + 10;   /* enough room */
    ensure_s_capacity(required);
    S_len = 0;

    for (idx_t m = 1; m <= max_m; ++m) {
        idx_t sq = m * m;
        if (sq <= N) {
            S[S_len++] = sq;
        }
        /* 3 * sq may overflow, guard */
        if (sq <= N / 3) {
            S[S_len++] = 3 * sq;
        }
    }
}

/* ---------- compute a(n) up to N ---------- */

static void compute_upto(idx_t N)
{
    if (N <= A_computed) return;

    ensure_a_capacity(N);
    build_S(N);

    for (idx_t n = A_computed + 1; n <= N; ++n) {
        seq_t sum = 0;
        for (size_t i = 0; i < S_len; ++i) {
            idx_t k = S[i];
            if (k >= n) continue;
            sum += A[n - k];
        }

        bool e_n = false;
        for (size_t i = 0; i < S_len; ++i) {
            if (S[i] == n) { e_n = true; break; }
        }
        seq_t term = e_n ? (seq_t)n : (seq_t)0;
        A[n] = term - sum;
    }
    A_computed = N;
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    /* offset is 1; a(1) is first term */
    if (n == 0) return 0;
    compute_upto(n);
    return A[n];
}
