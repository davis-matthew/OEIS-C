/*
 * A265891  a(n) = A099563(A000407(n)); the most significant digit in
 *          factorial base representation of (2n+1)! / n!.
 * https://oeis.org/A265891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A099563(A000407(n)) (most significant factorial‑base
 *            digit of (2 n+1)! / n!)
 * Range:     u8 n <= 249, u16 n <= 9166, u32 n <= 9166, u64 n <= 9166,
 *            u128 n <= 9166, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static double logfact[20003];
static int logfact_size = 20002; /* covers 0 .. 20002 */

/* Initialise the pre‑computed log10 factorial table.
   logfact[i] = log10(i!)  (logfact[0] = logfact[1] = 0). */
static void init_logfact(void) {
    static int done = 0;
    if (done) return;
    logfact[0] = 0.0;
    for (int i = 1; i <= logfact_size; ++i)
        logfact[i] = logfact[i - 1] + log10((double)i);
    done = 1;
}

/* Return the OEIS sequence A265891 value for index n (0‑based). */
seq_t a(idx_t n) {
    /* initialise log table once */
    init_logfact();

    /* 1.  Compute log10 of the product P = \prod_{k=n+1}^{2n+1} k
         Using logfact: log(P) = logfact[2n+1] - logfact[n] */
    idx_t up = 2 * n + 1;
    double logP = logfact[up] - logfact[n];

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
    double L = logP - logfact[m];            /* 0 <= L < log10(m+1) <= 5 */
    double d_guess = pow(10.0, L);
    /* Small epsilon to guard against rounding down by 1 */
    uint64_t d = (uint64_t)(d_guess + 1e-12);
    if (d == 0) d = 1;    /* safety, though not needed */

    /* 4.  Refine d using log comparison to avoid rounding errors */
    double logd = log10((double)d);
    double logdp1 = log10((double)(d + 1));

    /* If (d+1)*m! <= P, increase d */
    while (logfact[m] + logdp1 <= logP + 1e-12)
        ++d, logdp1 = log10((double)(d + 1));

    /* If d*m! > P, decrease d */
    while (d > 1 && (logfact[m] + logd > logP + 1e-12)) {
        --d;
        logd = log10((double)d);
    }

    return (seq_t)d;
}
