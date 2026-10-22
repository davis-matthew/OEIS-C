/*
 * A387205  a(n) = (n - 1)!*(2 + Harmonic(n - 1)) if n >= 1, and a(0) = 1.
 * https://oeis.org/A387205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * (2 + H_{n-1}), a(0) = 1 where H_{m} is the
 *            harmonic number. Implementation uses the recurrence fact[0] = 1,
 *            S[0] = 0 fact[i] = fact[i-1] * i S[i] = i * S[i-1] + fact[i-1]
 *            so that S[i] = i! * H_{i}. Then a(n) = 2*fact[n-1] + S[n-1]. The
 *            table is built on demand and extended when a...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

#ifdef __cplusplus
static std::vector<seq_t> fact;
static std::vector<seq_t> S;
#else
static seq_t *fact   = NULL;
static seq_t *S      = NULL;
static idx_t cache_size = 0;   /* number of entries stored (fact[0..cache_size-1]) */
#endif

/* Extend the table so that index m exists. */
static void grow_up_to(idx_t m)
{
#ifdef __cplusplus
    /* In C++ we can simply resize the vectors. */
    if (m + 1 <= fact.size())
        return;

    size_t old = fact.size();
    fact.resize(m + 1);
    S.resize(m + 1);

    for (idx_t i = old; i <= m; ++i) {
        if (i == 0) {
            fact[i] = 1;
            S[i]    = 0;
        } else {
            fact[i] = fact[i - 1] * (seq_t)i;
            S[i]    = (seq_t)i * S[i - 1] + fact[i - 1];
        }
    }
#else
    /* In C we use realloc.  seq_t is a plain integer type, so no
       construction is required. */
    if (m + 1 <= cache_size)
        return;

    idx_t new_size = cache_size ? cache_size * 2 : 1;
    if (new_size < (idx_t)(m + 1))
        new_size = m + 1;

    seq_t *new_fact = (seq_t *)realloc(fact, new_size * sizeof(seq_t));
    seq_t *new_S    = (seq_t *)realloc(S,    new_size * sizeof(seq_t));

    /* Assume allocation succeeds; the test harness never passes
       ridiculously large values that would overflow memory. */
    fact = new_fact;
    S    = new_S;

    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            fact[i] = 1;
            S[i]    = 0;
        } else {
            fact[i] = fact[i - 1] * (seq_t)i;
            S[i]    = (seq_t)i * S[i - 1] + fact[i - 1];
        }
    }
    cache_size = new_size;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t m = n - 1;
    grow_up_to(m);
    return fact[m] * 2 + S[m];
}
