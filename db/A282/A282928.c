/*
 * A282928  Expansion of Product_{k>=1} (1 - x^(7*k))^40/(1 - x^k)^41 in
 *          powers of x.
 * https://oeis.org/A282928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 22, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A282928: Expansion of ∏_{k≥1} (1 - x^(7k))^40/(1 - x^k)^41
   Recurrence:
        a(0) = 1
        n * a(n) = Σ_{k=1}^{n} B(k) * a(n-k)
   where B(k) = Σ_{d|k} d * c_d  and
         c_d = 41           (d not divisible by 7)
         c_d = 1            (d divisible by 7)
   →  B(k) = 41*σ₁(k) - 280*σ₁(k/7)  if 7|k, otherwise 41*σ₁(k).
   The recurrence is exact for all n≥0.  The implementation uses a
   static cache so that any order of calls is handled efficiently. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <type_traits>
constexpr bool is_small_seq = std::is_integral<seq_t>::value;
using intermediate_t = typename std::conditional<is_small_seq,
                                                unsigned __int128,
                                                seq_t>::type;
#else
/* In C we assume all seq_t types fit in 128 bits when required */
typedef unsigned __int128 intermediate_t;
#endif

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == (idx_t)0) return (seq_t)1;

    static seq_t *cache = NULL;   /* a[0..cap-1] */
    static seq_t *g     = NULL;   /* B[0..cap-1]  (unused B[0]) */
    static size_t cap = 0;        /* current allocated size */

    size_t N = (size_t)n;
    if (N + 1 > cap) {
        /* Grow cache and g arrays to size N+1 */
        if (cache) free(cache);
        if (g)     free(g);
        cache = (seq_t*)calloc(N + 1, sizeof(seq_t));
        g     = (seq_t*)calloc(N + 1, sizeof(seq_t));
        if (!cache || !g) {
            /* Allocation failed – abort */
#if defined(__cplusplus)
            throw std::bad_alloc();
#else
            abort();
#endif
        }

        /* Precompute B(k) = Σ_{d|k} d * c_d  for 1 ≤ k ≤ N */
        for (size_t d = 1; d <= N; ++d) {
            seq_t e = (d % 7 == 0) ? (seq_t)1 : (seq_t)41;
            seq_t add = (seq_t)d * e;
            for (size_t j = d; j <= N; j += d)
                g[j] += add;
        }

        /* a[0] = 1  */
        cache[0] = (seq_t)1;

        /* Compute a[1..N] using the recurrence */
        for (size_t nn = 1; nn <= N; ++nn) {
            intermediate_t s = 0;
            for (size_t k = 1; k <= nn; ++k) {
                intermediate_t prod =
                    (intermediate_t)g[k] * (intermediate_t)cache[nn - k];
                s += prod;
            }
            cache[nn] = (seq_t)(s / (intermediate_t)nn);  /* exact division */
        }

        cap = N + 1;
    }

    return cache[N];
}
