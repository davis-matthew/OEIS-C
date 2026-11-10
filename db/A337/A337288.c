/*
 * A337288  Numbers k such that k is in A095096 and k+1 is in A020899.
 * https://oeis.org/A337288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k >= 0 with k in A095096 (even number of Zeckendorf
 *            summands) and k+1 in A020899 (odd number); computed by a search
 *            with a cache
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9283 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define A337288_MAXF 90
static seq_t A337288_fibs[A337288_MAXF];
static int A337288_fc = 0;

static void A337288_extend(seq_t limit)
{
    if (A337288_fc == 0) {
        A337288_fibs[0] = 1;
        A337288_fibs[1] = 2;
        A337288_fc = 2;
    }
    while (A337288_fc < A337288_MAXF && A337288_fibs[A337288_fc - 1] <= limit) {
        seq_t nx = A337288_fibs[A337288_fc - 1] + A337288_fibs[A337288_fc - 2];
        if (nx < A337288_fibs[A337288_fc - 1]) break;
        A337288_fibs[A337288_fc++] = nx;
    }
}

/* returns number of Zeckendorf summands mod 2 */
static int A337288_par(seq_t k)
{
    int p = 0;
    if (k == 0) return 0;
    A337288_extend(k);
    int i = A337288_fc - 1;
    while (k > 0) {
        while (A337288_fibs[i] > k) --i;
        k -= A337288_fibs[i];
        p ^= 1;
    }
    return p;
}

static seq_t *A337288_cache = NULL;
static size_t A337288_cnt = 0, A337288_cap = 0;
static seq_t A337288_next = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A337288_cnt < (size_t)n) {
        seq_t k = A337288_next;
        A337288_next++;
        if (A337288_par(k) == 0 && A337288_par(k + 1) == 1) {
            if (A337288_cnt == A337288_cap) {
                size_t nc = A337288_cap ? A337288_cap * 2 : 64;
                if (A337288_cache == NULL) A337288_cache = mem_new(seq_t, nc);
                else A337288_cache = mem_resize(seq_t, A337288_cache, nc);
                A337288_cap = nc;
            }
            A337288_cache[A337288_cnt++] = k;
        }
    }
    return A337288_cache[n - 1];
}
