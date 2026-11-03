/*
 * A241926  Table read by antidiagonals: T(n,k) (n >= 1, k >= 1) is the number
 *          of necklaces with n black beads and k white beads.
 * https://oeis.org/A241926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the 2‑D table T(n,k) of cyclic
 *            necklaces with n black and k white beads by antidiagonals.
 *            T(n,k) = (Σ_{d | gcd(n,k)} φ(d)·binomial((n+k)/d, n/d)) / (n+k)
 * Range:     u8 n <= 24, u16 n <= 125, u32 n <= 796, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 94 terms through n = 1221 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Greatest common divisor for seq_t values. */
static seq_t gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

/* Compute binomial coefficient n choose k multiplicatively.
   This works for any seq_t that supports + - * / % with integer constants. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)      /* use symmetry */
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Compute T(n,k) from the given formula. */
static seq_t
table_T(idx_t n, idx_t k)
{
    if (n == 0 || k == 0)
        return 0;

    /* g = gcd(n,k) */
    idx_t g = (idx_t)gcd((seq_t)n, (seq_t)k);

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= g; ++d) {
        if (g % d == 0) {
            /* first divisor d */
            seq_t term = phi(d);                          /* φ(d)  */
            idx_t N = (n + k) / d;                       /* (n+k)/d */
            idx_t K = n / d;                             /*  n/d   */
            term = term * binomial(N, K);
            sum += term;

            /* second divisor g/d, if different */
            if (d != g / d) {
                idx_t e = g / d;
                term = phi(e);
                N = (n + k) / e;
                K = n / e;
                term = term * binomial(N, K);
                sum += term;
            }
        }
    }
    /* divide by (n+k) – this is an integer by the theory */
    return sum / (n + k);
}

/* a(n) – the n-th term of A241926 */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;

    /* find the diagonal d such that d*(d+1)/2 ≥ n */
    idx_t d = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++d;
        sum += d;
    }

    idx_t prev_sum = sum - d;                    /* S(d-1) */
    idx_t offset   = n - prev_sum - 1;           /* 0‑based offset inside diagonal */

    idx_t nb = offset + 1;                       /* n‑beads (black)  */
    idx_t kb = d - offset;                       /* k‑beads (white)  */

    return table_T(nb, kb);
}
