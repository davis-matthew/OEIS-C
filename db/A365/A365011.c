/*
 * A365011  E.g.f. satisfies A(x) = 1 + x*exp(-x)*A(x)^4.
 * https://oeis.org/A365011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} (-k)^(n-k) * C(4k,k)/(3k+1)/(n-k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binom, ensure_cache, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow_uint.h"
#include "../utils/ensure_cache.h"

/*  Compute a(n) for OEIS sequence A365011  */
seq_t a(idx_t n)
{
    /* ---------- factorial cache ---------- */
    static seq_t *fact = NULL;
    static size_t fact_sz = 0;
    ensure_cache(n, &fact, &fact_sz);          /* obtain array of at least n+1 */
    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i-1] * (seq_t)i;          /* n!                   */

    /* ---------- accumulate positive and negative parts ---------- */
    seq_t pos = 0;       /* sum of terms with positive sign  */
    seq_t neg = 0;       /* sum of terms with negative sign  */

    seq_t fact_n = fact[n];                       /* n! */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t exp = n - k;                         /* exponent of (-k)   */
        /* sign of (-k)^exp : (+1 for even exp, -1 for odd exp) */
        bool neg_term = (exp & 1U);               /* true -> negative   */

        /* skip term k==0 with exp>0 : contributes 0 */
        if (k == 0 && exp > 0) continue;

        /* P = n! / (n-k)!  (integer division) */
        seq_t P = fact_n / fact[exp];

        /* k^(n-k)  (base may be 0) */
        seq_t powk = 1;
        if (k != 0 && exp > 0)
            powk = pow_uint((uint64_t)k, exp);

        /* B_k = C(4k,k)/(3k+1)  (Fuss–Catalan number) */
        idx_t fourk = k << 2;
        seq_t bin = binom(fourk, k);
        seq_t Bk = bin / ((3 * (seq_t)k) + 1U);

        /* term magnitude = P * powk * Bk   (all integers) */
        seq_t term = P * powk;
        term *= Bk;

        /* add to positive or negative sum */
        if (neg_term)
            neg += term;
        else
            pos += term;
    }

    /* final result = pos - neg  ;  pos is guaranteed ≥ neg   */
    return pos - neg;
}
