/*
 * A304873  G.f.: Sum_{k>=0} p(k)^4 * x^k / Sum_{k>=0} p(k)*x^k, where p(n) is
 *          the partition function A000041(n).
 * https://oeis.org/A304873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 45, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = [x^n] ( Σ_{k≥0} p(k)^4 x^k ) / ( Σ_{k≥0} p(k) x^k )
 * where p(n) is the partition function A000041(n).
 * The computation uses the convolution identity
 *   a(n) = p(n)^4 − Σ_{k=1..n} a(n−k) * p(k)
 * with a(0)=1, p(0)=1.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

static idx_t curN = 0;
static int initialized = 0;

#ifdef __cplusplus
static std::vector<seq_t> p_vals;
static std::vector<seq_t> c_vals;
#else
static seq_t *p_vals = NULL;
static seq_t *c_vals = NULL;
#endif

/* Allocate the initial 1‑element arrays */
static void init(void)
{
    if (initialized) return;
    initialized = 1;
    curN = 0;
#ifdef __cplusplus
    p_vals.resize(1);
    c_vals.resize(1);
#else
    p_vals = (seq_t *)malloc(sizeof(seq_t));
    c_vals = (seq_t *)malloc(sizeof(seq_t));
    if (!p_vals || !c_vals) exit(EXIT_FAILURE);
#endif
    p_vals[0] = 1;
    c_vals[0] = 1;
}

/* Ensure the partitions and sequence are computed up to N. */
static void ensure_size(idx_t N)
{
    if (N <= curN) return;
    idx_t old = curN;
    curN = N;
#ifdef __cplusplus
    p_vals.resize(N + 1);
    c_vals.resize(N + 1);
#else
    seq_t *tmp_p = (seq_t *)realloc(p_vals, (N + 1) * sizeof(seq_t));
    seq_t *tmp_c = (seq_t *)realloc(c_vals, (N + 1) * sizeof(seq_t));
    if (!tmp_p || !tmp_c) exit(EXIT_FAILURE);
    p_vals = tmp_p;
    c_vals = tmp_c;
#endif

    /* p[0] and a[0] are already set to 1 by init(); overwrite harmlessly. */
    p_vals[0] = 1;
    c_vals[0] = 1;

    /* Compute partition numbers p[n] for n = old+1 .. N */
    for (idx_t n = old + 1; n <= N; ++n) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n) break;
            seq_t val = p_vals[(size_t)(n - pent1)];
            if (k & 1) pos += val; else neg += val;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > n) continue;
            val = p_vals[(size_t)(n - pent2)];
            if (k & 1) pos += val; else neg += val;
        }
        p_vals[(size_t)n] = pos - neg;
    }

    /* Compute a[n] for n = old+1 .. N using the convolution identity */
    for (idx_t n = old + 1; n <= N; ++n) {
        seq_t an = p_vals[(size_t)n];
        an = an * an; /* p(n)^2 */
        an = an * an; /* p(n)^4 */
        seq_t sum = 0;
        for (idx_t k = 1; k <= n; ++k) {
            sum += c_vals[(size_t)(n - k)] * p_vals[(size_t)k];
        }
        c_vals[(size_t)n] = an - sum;
    }
}

seq_t a(idx_t n)
{
    init();
    if (n > curN) ensure_size(n);
    return c_vals[(size_t)n];
}
