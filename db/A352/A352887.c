/*
 * A352887  Expansion of e.g.f. 1/(1 - Sum_{k>=1} phi(k)*x^k/k!), where phi is
 *          the Euler totient function A000010.
 * https://oeis.org/A352887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n)=∑_{k=1..n} φ(k)*C(n,k)*a(n‑k) */
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/phi.h"

static seq_t *a_cache = NULL;          /* cache for a(1)…a(n); a(0) is implicit 1   */
static size_t a_cap   = 0;
static idx_t   a_last = 0;

static seq_t *phi_cache = NULL;        /* cache for φ(1)…φ(n)   */
static size_t phi_cap   = 0;

/* Compute a(n) using the recurrence and memoising results.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Ensure phi_cache contains φ(k) for 1 ≤ k ≤ n. */
    if (phi_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(phi_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        phi_cache = tmp;
        for (idx_t i = 1; i <= n; ++i) phi_cache[i] = (seq_t)i;
        for (idx_t p = 2; p <= n; ++p)
            if (phi_cache[p] == (seq_t)p)
                for (idx_t j = p; j <= n; j += p)
                    phi_cache[j] -= phi_cache[j] / p;
        phi_cap = new_cap;
    }

    /* Ensure a_cache covers 1..n. */
    if (a_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        for (idx_t i = a_cap; i <= n; ++i) a_cache[i] = (seq_t)0;
        a_cap = new_cap;
    }

    /* Compute missing values up to n. */
    idx_t start = a_last < 1 ? 1 : a_last + 1;
    for (idx_t i = start; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t phik = phi_cache[k];
            seq_t bin  = binomial(i, k);
            seq_t sub  = (k == i) ? (seq_t)1 : a_cache[i - k];
            sum += phik * bin * sub;
        }
        a_cache[i] = sum;
    }
    a_last = n;
    return a_cache[n];
}
