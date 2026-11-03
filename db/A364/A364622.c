/*
 * A364622  G.f. satisfies A(x) = 1/(1-x)^2 + x^2*A(x)^4.
 * https://oeis.org/A364622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n+4*k+1,6*k+1) *
 *            binomial(4*k,k) / (3*k+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static seq_t *B = NULL;      /* B[k] holds binomial(n+4k+1,6k+1) for current n */
static size_t B_cap = 0;

static seq_t *C = NULL;      /* C[k] = binomial(4k,k)/(3k+1) */
static size_t C_cap = 0;

static void ensure_cache_cap(size_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= n) new_cap <<= 1;
    seq_cache = mem_resize(seq_t, seq_cache, new_cap);
    mem_set(seq_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache_cap = new_cap;
}

static void ensure_B_cap(size_t k)
{
    if (B_cap > k) return;
    size_t new_cap = B_cap ? B_cap << 1 : 16;
    while (new_cap <= k) new_cap <<= 1;
    B = mem_resize(seq_t, B, new_cap);
    mem_set(B + B_cap, 0, (new_cap - B_cap) * sizeof(seq_t));
    B_cap = new_cap;
}

static void ensure_C_cap(size_t k)
{
    if (C_cap > k) return;
    size_t new_cap = C_cap ? C_cap << 1 : 16;
    while (new_cap <= k) new_cap <<= 1;
    C = mem_resize(seq_t, C, new_cap);
    mem_set(C + C_cap, 0, (new_cap - C_cap) * sizeof(seq_t));
    C_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n >= cache_len) {
        size_t max_k_needed = n >> 1;
        ensure_cache_cap(n);
        ensure_B_cap(max_k_needed);
        ensure_C_cap(max_k_needed);

        for (idx_t m = cache_len; m <= n; ++m) {
            idx_t kmax = m >> 1;

            /* compute any missing C[k] values */
            for (idx_t k = 1; k <= kmax; ++k) {
                if (C[k] == 0) {
                    seq_t tmp = binomial(4 * k, k);
                    C[k] = tmp / (3 * k + 1);
                }
            }

            /* update B[k] for all k <= kmax */
            for (idx_t k = 1; k <= kmax; ++k) {
                if (m == 2 * k) {
                    B[k] = 1; /* binomial(6k+1,6k+1) = 1 */
                } else if (m > 2 * k) {
                    /* B[k] = B[k] * (m + 4*k + 1) / (m - 2*k) */
                    seq_t num = B[k] * (m + 4 * k + 1);
                    B[k] = num / (m - 2 * k);
                }
            }

            seq_t val = (seq_t)m + 1;   /* accounts for k=0 term */
            for (idx_t k = 1; k <= kmax; ++k) {
                val += B[k] * C[k];
            }
            seq_cache[m] = val;
        }
        cache_len = n + 1;
    }
    return seq_cache[n];
}
