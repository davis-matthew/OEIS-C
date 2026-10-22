/*
 * A377036  First term of the n-th differences of the composite numbers.
 *          Inverse zero-based binomial transform of A002808.
 * https://oeis.org/A377036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) * C_k where C_k
 *            denotes the k‑th composite number (C_0 = 4)
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 29, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Simple trial division primality test, adequate for the small integers
   that we generate when collecting composite numbers. */
static bool is_small_prime(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

#ifdef __cplusplus
static std::vector<seq_t> composites;
#else
static seq_t *composites   = NULL;
static size_t composites_sz = 0;
static size_t composites_cap = 0;
#endif
static idx_t next_composite = 4;   /* next integer to test for compositeness */

/* Ensure that the first (n+1) composite numbers are stored. */
static void ensure_composites(idx_t n)
{
#ifdef __cplusplus
    while (composites.size() <= n) {
        bool prime = is_small_prime(next_composite);
        if (!prime) composites.push_back((seq_t)next_composite);
        ++next_composite;
    }
#else
    while (composites_sz <= n) {
        bool prime = is_small_prime(next_composite);
        if (!prime) {
            if (composites_sz == composites_cap) {
                size_t new_cap = composites_cap ? composites_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
                composites = tmp;
                composites_cap = new_cap;
            }
            composites[composites_sz++] = (seq_t)next_composite;
        }
        ++next_composite;
    }
#endif
}

/* External linkage function required by the problem statement. */
seq_t a(idx_t n)
{
    ensure_composites(n);

    seq_t result = 0;
    seq_t binom   = 1;   /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t qk;
#ifdef __cplusplus
        qk = composites[k];
#else
        qk = composites[k];
#endif
        seq_t term = qk * binom;
        if ((n - k) & 1) term = -term;
        result += term;

        if (k < n) {
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);   /* C(n,k+1) */
        }
    }

    return result;
}
