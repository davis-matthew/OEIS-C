/*
 * A387135  Number of distinct prime factors in common to all prime indices of
 *          n.
 * https://oeis.org/A387135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors that are common to all
 *            prime indices of n. Prime indices of n are the indices m such
 *            that prime(m) divides n. The function uses only standard C/C++
 *            libraries and the build‑provided helper compute_pi which returns
 *            π(n) for any n. It factors integers via simp...
 * Range:     u8 n <= 5896, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_pi.h"

/* Return the distinct prime factors of x in ascending order.
   `factors` must be able to hold at least 64 elements.  The number of
   distinct factors is returned via *cnt. */
static void prime_factors_uint64(uint64_t x, uint64_t *factors, size_t *cnt)
{
    size_t i = 0;
    if (x <= 1) {
        *cnt = 0;
        return;
    }
    if (x % 2 == 0) {
        factors[i++] = 2;
        while (x % 2 == 0) x /= 2;
    }
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) {
            factors[i++] = d;
            while (x % d == 0) x /= d;
        }
    }
    if (x > 1) factors[i++] = x;
    *cnt = i;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* defined only for n ≥ 1 */

    /* Factors of n */
    uint64_t n_factors[64];
    size_t n_cnt;
    prime_factors_uint64((uint64_t)n, n_factors, &n_cnt);

    if (n_cnt == 0) return (seq_t)0;      /* n == 1 */

    /* Intersection of prime factors of all prime indices of n */
    uint64_t inter[64];
    size_t inter_cnt = 0;

    bool first = true;
    for (size_t i = 0; i < n_cnt; ++i) {
        /* Get the prime index of the prime factor */
        idx_t m = (idx_t)(seq_t)compute_pi(n_factors[i]);   /* π(p) */

        /* Prime factors of the index */
        uint64_t m_factors[64];
        size_t m_cnt;
        prime_factors_uint64((uint64_t)m, m_factors, &m_cnt);

        if (first) {
            inter_cnt = m_cnt;
            for (size_t k = 0; k < inter_cnt; ++k) inter[k] = m_factors[k];
            first = false;
        } else {
            uint64_t new_inter[64];
            size_t new_cnt = 0;
            for (size_t a = 0; a < inter_cnt; ++a) {
                for (size_t b = 0; b < m_cnt; ++b) {
                    if (inter[a] == m_factors[b]) {
                        new_inter[new_cnt++] = inter[a];
                        break;
                    }
                }
            }
            inter_cnt = new_cnt;
            if (inter_cnt == 0) break;   /* no common factors */
            for (size_t k = 0; k < inter_cnt; ++k) inter[k] = new_inter[k];
        }
    }

    return (seq_t)inter_cnt;
}
