/*
 * A394324  Number of partitions p of n such that (minimal multiplicity of the
 *          parts of p) >= (maximal part of p).
 * https://oeis.org/A394324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The generating function is sum_{j>=1} q^(j^2)/(1-q^j) *
 *            prod_{k=1}^{j-1} (1+q^(j*k)/(1-q^k)) → partitions of n where the
 *            minimal multiplicity of parts ≥ maximal part
 * Range:     u8 n <= 49, u16 n <= 149, u32 n <= 534, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    const size_t N = (size_t)n;

    /* allocate temporaries */
    seq_t *f, *new_f;
#if defined(__cplusplus)
    f     = new seq_t[N + 1]();
    new_f = new seq_t[N + 1]();
#else
    f     = (seq_t*)calloc(N + 1, sizeof(seq_t));
    new_f = (seq_t*)calloc(N + 1, sizeof(seq_t));
#endif
    if (!f || !new_f) return 0;    /* allocation failure guard */

    /* maximal j such that j^2 <= N */
    size_t max_j = 0;
    while ((max_j + 1ULL) * (max_j + 1ULL) <= N) ++max_j;

    seq_t sum = 0;                 /* answer accumulator */

    /* iterate over possible maximal parts j */
    for (size_t j = 1; j <= max_j; ++j) {
        /* zero f */
        for (size_t i = 0; i <= N; ++i) f[i] = 0;

        /* base term q^(j^2)/(1-q^j) → coeff 1 at exponents t*j with t >= j */
        for (size_t t = j; t * j <= N; ++t) f[t * j] = 1;

        /* multiply by factors for parts k < j */
        for (size_t k = 1; k < j; ++k) {
            /* zero new_f for this k */
            for (size_t i = 0; i <= N; ++i) new_f[i] = 0;

            for (size_t r = 0; r < k; ++r) {
                size_t limit = (N - r) / k;  /* largest t s.t. r + t*k <= N */

                /* prefix sums of f over this residue */
                seq_t *pref;
#if defined(__cplusplus)
                pref = new seq_t[limit + 1]();
#else
                pref = (seq_t*)calloc(limit + 1, sizeof(seq_t));
#endif
                pref[0] = f[r];
                for (size_t t = 1; t <= limit; ++t)
                    pref[t] = pref[t - 1] + f[r + t * k];

                /* compute new_f using: new[idx] = f[idx] + pref[t-j] if t>=j */
                for (size_t t = 0; t <= limit; ++t) {
                    size_t idx = r + t * k;
                    if (t >= j)
                        new_f[idx] = f[idx] + pref[t - j];
                    else
                        new_f[idx] = f[idx];
                }

#if defined(__cplusplus)
                delete[] pref;
#else
                free(pref);
#endif
            }

            /* swap f and new_f for the next k */
            seq_t *tmp = f; f = new_f; new_f = tmp;
        }

        /* add contribution of this j to the final sum */
        sum += f[N];
    }

#if defined(__cplusplus)
    delete[] f;
    delete[] new_f;
#else
    free(f);
    free(new_f);
#endif

    return sum;
}
