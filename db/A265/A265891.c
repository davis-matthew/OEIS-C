/*
 * A265891  a(n) = A099563(A000407(n)); the most significant digit in
 *          factorial base representation of (2n+1)! / n!.
 * https://oeis.org/A265891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A099563(A000407(n)) (most significant factorial‑base
 *            digit of (2 n+1)! / n!)
 * Range:     u8 n <= 249, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 109 terms through n = 9166 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

/* log10(i!) for i < logfact_len; grows as needed (the original had a fixed
   20003 entries and read past them from n = 10001 on) */
static long double *logfact = NULL;
static idx_t logfact_len = 0;

/* make logfact[0..need] available */
static void init_logfact(idx_t need) {
    if (need < logfact_len) return;
    idx_t len = logfact_len ? logfact_len : 1024;
    while (len <= need) len *= 2;
    long double *t = (long double *)realloc(logfact, (size_t)len * sizeof *t);
    if (!t) abort();
    logfact = t;
    if (logfact_len == 0) logfact[logfact_len++] = 0.0L;
    for (idx_t i = logfact_len; i < len; ++i) logfact[i] = logfact[i - 1] + log10l((long double)i);
    logfact_len = len;
}

/* Return the OEIS sequence A265891 value for index n (0‑based). */
seq_t a(idx_t n) {
    init_logfact(2 * n + 2);

    /* 1.  Compute log10 of the product P = \prod_{k=n+1}^{2n+1} k
         Using logfact: log(P) = logfact[2n+1] - logfact[n] */
    idx_t up = 2 * n + 1;
    long double logP = logfact[up] - logfact[n];

    /* 2.  Find m = floor largest i with logfact[i] <= logP */
    idx_t low = 0, high = up;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (logfact[mid] <= logP)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t m = low;            /* m! <= P < (m+1)! */

    /* 3.  Compute d = floor(P / m!) without large integers.
           The logarithm of the quotient is logP - logfact[m].
           Compute 10^(logP - logfact[m]) and floor it. */
    long double L = logP - logfact[m];       /* 0 <= L < log10(m+1) <= 5 */
    long double d_guess = powl(10.0L, L);
    /* Small epsilon to guard against rounding down by 1 */
    uint64_t d = (uint64_t)(d_guess + 1e-12L);
    if (d == 0) d = 1;    /* safety, though not needed */

    /* 4.  Refine d using log comparison to avoid rounding errors */
    long double logd = log10l((long double)d);
    long double logdp1 = log10l((long double)(d + 1));

    /* If (d+1)*m! <= P, increase d */
    while (logfact[m] + logdp1 <= logP + 1e-12L)
        ++d, logdp1 = log10l((long double)(d + 1));

    /* If d*m! > P, decrease d */
    while (d > 1 && (logfact[m] + logd > logP + 1e-12L)) {
        --d;
        logd = log10l((long double)d);
    }

    return (seq_t)d;
}
