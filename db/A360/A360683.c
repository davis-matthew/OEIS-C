/*
 * A360683  Number of integer partitions of n whose second differences sum to
 *          0, meaning either there is only one part, or the first two parts
 *          have the same difference as the last two parts.
 * https://oeis.org/A360683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer partitions of n whose first *
 *            difference (λ1-λ2) equals the last difference (λ_{k-1}-λ_k), *
 *            or n=0 (empty partition). The sequence is computed by *
 *            transforming the partition differences f_i and summing the *
 *            generating function contributions
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -- include needed headers -------------------------------------------------- */
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */

/* -- function --------------------------------------------------------------- */
seq_t a(idx_t N)
{
    if (N == 0)                     /* a(0) = 1 */
        return (seq_t)1;

    size_t n = (size_t)N;           /* n ≤ 10000, safe for size_t */

    /* allocate arrays for counts */
    seq_t *cnt   = (seq_t *)malloc((n + 1) * sizeof(seq_t));  /* a[]  */
    seq_t *part  = (seq_t *)malloc((n + 1) * sizeof(seq_t));  /* curP[]*/

    /* initialisation */
    for (size_t i = 0; i <= n; ++i) {
        cnt[i]  = (seq_t)0;
        part[i] = (seq_t)0;
    }
    cnt[0] = (seq_t)1;          /* empty partition */

    /* single‑part partitions:  a[i] += 1 */
    for (size_t i = 1; i <= n; ++i)
        cnt[i] += (seq_t)1;

    /* two‑part partitions:  a[i] += floor(i/2) */
    for (size_t i = 2; i <= n; ++i)
        cnt[i] += (seq_t)(i / 2);

    /* initialise partition generating function for parts ≥ 2   */
    part[0] = (seq_t)1;          /* only empty sum uses no parts */

    /* contributions from partitions with ≥3 parts */
    for (size_t k = 3; k <= n; ++k) {

        size_t m = k - 2;                 /* new part size to include */

        if (m >= 2) {                     /* update part[] (parts up to m) */
            for (size_t i = m; i <= n; ++i)
                part[i] += part[i - m];
        }

        size_t maxS = n - k;               /* remaining exponent after q^k */

        for (size_t s = 0; s <= maxS; ++s) {
            seq_t base = part[s];
            if (base == (seq_t)0)          /* skip zero coefficients */
                continue;

            size_t tmax = (maxS - s) / k;
            size_t idx  = k + s;           /* current exponent n = k + s + t*k */
            seq_t coeff = (seq_t)1;

            for (size_t t = 0; t <= tmax; ++t, idx += k, ++coeff) {
                cnt[idx] += base * coeff;  /* add contribution to a[idx] */
            }
        }
    }

    seq_t result = cnt[n];

    free(cnt);
    free(part);

    return result;
}
