/*
 * A364819  Number of permutations p of [n] such that max(|p(p(i)) - i|)=1.
 * https://oeis.org/A364819
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊n/4⌋} (n-2k)! * A000085(n-4k) / (k! * (n-4k)!)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define MAX_N 20000  /* should cover all expected n (≈10000) */

/* Cache of involution numbers A000085[i] = number of involutions on i elements */
static seq_t invol[MAX_N + 1];
static idx_t invol_computed_up_to = 0;

/* Ensure the invol array contains values up to n */
static void ensure_invol(idx_t n)
{
    if (n > MAX_N) {              /* guard against out‑of‑range requests */
        return;
    }
    if (invol_computed_up_to >= n) {
        return;
    }
    if (invol_computed_up_to == 0) {
        invol[0] = 1;
        if (MAX_N >= 1) invol[1] = 1;
        invol_computed_up_to = 1;
    }
    for (idx_t i = invol_computed_up_to + 1; i <= n; ++i) {
        invol[i] = invol[i - 1] + (seq_t)(i - 1) * invol[i - 2];
    }
    invol_computed_up_to = n;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* behaviour below offset is irrelevant */

    /* make sure we have involutions up to n */
    ensure_invol(n);

    idx_t maxk = n / 4;
    seq_t ans = 0;
    seq_t fact_k = 1;              /* 0! = 1 → for k=1 we use 1! */

    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t m = n - 4 * k;       /* n-4k */

        /* compute numerator (n-2k)! / (n-4k)! = product_{i=m+1}^{m+2k} i */
        seq_t prod = 1;
        for (idx_t j = m + 1; j <= m + 2 * k; ++j) {
            prod *= (seq_t)j;
        }

        /* term = prod * invol[m] / k! */
        seq_t term = prod * invol[m];
        term /= fact_k;  /* exact division */

        ans += term;

        fact_k *= (seq_t)(k + 1);  /* prepare (k+1)! for the next round */
    }

    return ans;
}
