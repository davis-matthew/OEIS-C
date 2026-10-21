/*
 * A245269  Sum of binomial(n,k) over cubefree k.
 * https://oeis.org/A245269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n, k cubefree} binomial(n,k)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   binom_next, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (cubefree means no prime factor occurs with exponent ≥ 3) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/binom_next.h"

/* Cached smallest prime factors up to the largest n requested so far */
static idx_t *SPF = NULL;
static idx_t  SPF_MAX = 0;

/* Ensure that SPF contains smallest prime factors for all numbers ≤ n */
static void
ensure_spf_upto(idx_t n)
{
    if (n <= SPF_MAX) return;
    if (SPF != NULL) free(SPF);
    /* build_spf() allocates an array of size n+1 (0..n) and
       sets SPF[i] = smallest prime dividing i (SPF[0], SPF[1] are 0). */
    build_spf(n, &SPF);
    SPF_MAX = n;
}

/* Return true iff k is cubefree */
static inline int
is_cubefree(idx_t k)
{
    while (k > 1) {
        idx_t p = SPF[k];
        idx_t exp = 0;
        while (k % p == 0) {
            k /= p;
            exp++;
            if (exp >= 3) return 0;
        }
    }
    return 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety: a(0) is out of range */

    /* Prepare spf table up to n for cubefree tests */
    ensure_spf_upto(n);

    seq_t sum   = 0;
    seq_t binom = 1;                      /* binomial(n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        binom = binom_next(binom, n, k);  /* binomial(n,k) */
        if (is_cubefree(k))
            sum += binom;
    }
    return sum;
}
