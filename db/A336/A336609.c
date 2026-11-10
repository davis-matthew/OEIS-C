/*
 * A336609  Sum_{n>=0} a(n) * x^n / (n!)^2 = exp(1 / BesselJ(0,2*sqrt(x)) -
 *          1).
 * https://oeis.org/A336609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} binomial(n,k)^2 * k * A000275(k) *
 *            a(n-k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A336609_MAX 2001

static seq_t A336609_f[A336609_MAX]; /* A000275 values */
static seq_t A336609_memo[A336609_MAX];
static idx_t A336609_fcount = 0;
static idx_t A336609_count = 0;

static void A336609_fill_f(idx_t n)
{
    idx_t m, r;
    if (A336609_fcount == 0) {
        A336609_f[0] = 1;
        A336609_fcount = 1;
    }
    for (m = A336609_fcount; m <= n; m++) {
        seq_t pos = 0, neg = 0;
        seq_t c = 1;
        for (r = 0; r < m; r++) {
            seq_t t = c * c * A336609_f[r];
            if (((r + m + 1) & 1) == 0)
                pos += t;
            else
                neg += t;
            c = c * (seq_t)(m - r) / (seq_t)(r + 1);
        }
        A336609_f[m] = pos - neg;
        A336609_fcount = m + 1;
    }
}

seq_t a(idx_t n)
{
    idx_t m, k;
    if (n >= A336609_MAX)
        n = A336609_MAX - 1;
    A336609_fill_f(n);
    if (A336609_count == 0) {
        A336609_memo[0] = 1;
        A336609_count = 1;
    }
    for (m = A336609_count; m <= n; m++) {
        seq_t sum = 0;
        seq_t c = 1; /* binomial(m, k) */
        for (k = 1; k <= m; k++) {
            c = c * (seq_t)(m - k + 1) / (seq_t)k;
            sum += c * c * (seq_t)k * A336609_f[k] * A336609_memo[m - k];
        }
        A336609_memo[m] = sum / (seq_t)m;
        A336609_count = m + 1;
    }
    return A336609_memo[n];
}
