/*
 * A272498  Number of ordered set partitions of [n] with nondecreasing block
 *          sizes and maximal block size equal to eight.
 * https://oeis.org/A272498
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n,8) – f(n,7) where f(0,s) = 1 and for m>0, f(m,s) =
 *            Σ_{t=1}^{min(s,m)} C(m,t) * f(m‑t,t) (s = 1…8)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 16, u64 n <= 23, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* --- Helper: binomial coefficient C(n,k) for k ≤ 8, computed safely -- */
static seq_t binom(size_t n, size_t k)
{
    seq_t r = 1;
    for (size_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i);
        r = r / (seq_t)i;               /* exact division */
    }
    return r;
}

/* --- DP tables: f[n*9 + s]  (0 ≤ n ≤ cap, 1 ≤ s ≤ 8)  --- */
static seq_t *f      = NULL;      /* flat array, length (cap+1)*9   */
static size_t f_cap  = 0;        /* largest n computed (inclusive) */

/* Allocate / grow tables up to max_n (inclusive) and compute f values. */
static void ensure_dp(size_t max_n)
{
    if (max_n <= f_cap) return;                  /* already available */

    size_t old_cap = f_cap;
    f_cap           = max_n;

    /* Resize arrays */
    f = (seq_t *)realloc(f, (f_cap + 1) * 9 * sizeof(seq_t));

    /* Initialize new rows to zero */
    for (size_t n = old_cap + 1; n <= f_cap; ++n)
        for (int s = 1; s <= 8; ++s)
            f[n * 9 + s] = 0;

    /* Base case: f(0,s) = 1 for all s = 1..8 */
    for (int s = 1; s <= 8; ++s)
        f[0 * 9 + s] = 1;

    /* Compute DP */
    for (size_t n = 1; n <= f_cap; ++n) {
        for (int s = 1; s <= 8; ++s) {
            seq_t total = 0;
            size_t max_t = n < (size_t)s ? n : (size_t)s;
            for (size_t t = 1; t <= max_t; ++t) {
                seq_t comb = binom(n, t);
                seq_t prev = f[(n - t) * 9 + t];
                total += comb * prev;
            }
            f[n * 9 + s] = total;
        }
    }
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    if ((idx_t)8 > n)             /* sequence only defined for n ≥ 8 */
        return 0;

    ensure_dp((size_t)n);         /* guarantee that f[n] is known */

    /* a(n) = f(n,8) - f(n,7) */
    seq_t val_8 = f[n * 9 + 8];
    seq_t val_7 = f[n * 9 + 7];
    return val_8 - val_7;
}
