/*
 * A281012  Zig-zag path that we can find descending by the main diagonal of
 *          the pyramid described in A245092.
 * https://oeis.org/A281012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with m(k) = k-th term of A071562 (integers with a middle
 *            divisor, found by a sieve/search), a(2k-1) = A067742(m(k))
 *            (number of divisors d of m(k) with m(k)/2 <= d^2 < 2*m(k)),
 *            a(2k) = m(k+1) - m(k)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 104 terms through n = 104 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *m_vals = NULL;
static uint64_t *c_vals = NULL;
static uint64_t m_len = 0, m_cap = 0, m_next = 1;

static uint64_t mid_count(uint64_t m)
{
    uint64_t d = 1;
    uint64_t c = 0;
    while (2 * d * d < m) d++;
    for (; d * d < 2 * m; d++) {
        if (m % d == 0) c++;
    }
    return c;
}

static void extend_to(uint64_t k)
{
    while (m_len < k) {
        uint64_t c = mid_count(m_next);
        if (c > 0) {
            if (m_len == m_cap) {
                uint64_t nc = m_cap ? m_cap * 2 : 64;
                m_vals = (uint64_t *)realloc(m_vals, nc * sizeof(uint64_t));
                c_vals = (uint64_t *)realloc(c_vals, nc * sizeof(uint64_t));
                if (!m_vals || !c_vals) abort();
                m_cap = nc;
            }
            m_vals[m_len] = m_next;
            c_vals[m_len] = c;
            m_len++;
        }
        m_next++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t k = ((uint64_t)n + 1) / 2;   /* 1-based block index */
    if (n & 1) {
        extend_to(k);
        return (seq_t)c_vals[k - 1];
    } else {
        extend_to(k + 1);
        return (seq_t)(m_vals[k] - m_vals[k - 1]);
    }
}
