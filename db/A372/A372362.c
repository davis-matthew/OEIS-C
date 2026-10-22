/*
 * A372362  a(n) is the 2-adic valuation of 1 + 3*{the n-th iterate of 27 with
 *          Reduced Collatz-function R}.
 * https://oeis.org/A372362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v₂(1 + 3 * Rⁿ(27)), where R(x) = (3x+1)/2ᵏ with 2ᵏ the
 *            largest power of 2 dividing (3x+1). Offset: 0 (a(0)=1)
 * Range:     u8 n <= 8, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *m_vals = NULL;   /* array of successive Rⁿ(27) values, all odd. */
static idx_t m_len  = 0;       /* number of entries currently stored. */
static size_t m_cap = 0;       /* allocated capacity in seq_t elements. */

/* Ensure that at least 'req' elements can be stored (indices 0..req-1). */
static void ensure_m_cap(idx_t req)
{
    if (m_cap >= (size_t)req) return;

    size_t new_cap = m_cap ? m_cap * 2 : 16;
    while (new_cap < (size_t)req) new_cap <<= 1;

    m_vals = (seq_t *)realloc(m_vals, new_cap * sizeof(seq_t));
    m_cap = new_cap;
}

/* Compute the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Initialise storage with the first iterate R⁰(27)=27. */
    if (m_len == 0) {
        ensure_m_cap(1);
        m_vals[0] = (seq_t)27;
        m_len = 1;
    }

    /* Extend the cache up to the desired index. */
    if (n >= m_len) {
        ensure_m_cap(n + 1);                 /* capacity for indices 0..n */

        for (idx_t i = m_len; i <= n; ++i) {
            seq_t prev = m_vals[(size_t)(i - 1)];
            seq_t t    = (seq_t)3 * prev + (seq_t)1;
            while ((t & (seq_t)1) == (seq_t)0) t >>= 1; /* strip powers of 2 */
            m_vals[(size_t)i] = t;
        }
        m_len = n + 1;
    }

    /* At this point m_vals[n] is defined.  Compute its valuation. */
    seq_t m = m_vals[(size_t)n];
    seq_t x = (seq_t)1 + (seq_t)3 * m;
    idx_t  v = 0;
    while ((x & (seq_t)1) == (seq_t)0) {
        x >>= 1;
        ++v;
    }
    return (seq_t)v;
}
