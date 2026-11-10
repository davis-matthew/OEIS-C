/*
 * A355606  The indices where A354606(n) = 1.
 * https://oeis.org/A355606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index k with A354606(k) = 1, where A354606(1)=1 and
 *            A354606(k) = #{j<k : tau(A354606(j)) = tau(A354606(k-1))};
 *            computed by simulation
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-10)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

static uint64_t *A355606_ones = NULL;
static size_t A355606_cnt = 0, A355606_cap = 0;
static uint64_t *A355606_freq = NULL;
static size_t A355606_fcap = 0;
static uint64_t A355606_i = 0;     /* number of A354606 terms generated */
static uint64_t A355606_prev = 0;  /* last A354606 term */

static void A355606_ensure_freq(size_t idx)
{
    if (idx < A355606_fcap) return;
    size_t nc = A355606_fcap ? A355606_fcap * 2 : 64;
    while (nc <= idx) nc *= 2;
    uint64_t *t = (uint64_t *)realloc(A355606_freq, nc * sizeof(uint64_t));
    if (!t) exit(EXIT_FAILURE);
    for (size_t k = A355606_fcap; k < nc; ++k) t[k] = 0;
    A355606_freq = t;
    A355606_fcap = nc;
}

static void A355606_step(void)
{
    uint64_t val;
    A355606_i++;
    if (A355606_i == 1) {
        val = 1;
    } else {
        size_t d = (size_t)num_divisors(A355606_prev);
        A355606_ensure_freq(d);
        val = A355606_freq[d];
    }
    if (val == 1) {
        if (A355606_cnt == A355606_cap) {
            size_t nc = A355606_cap ? A355606_cap * 2 : 64;
            uint64_t *t = (uint64_t *)realloc(A355606_ones, nc * sizeof(uint64_t));
            if (!t) exit(EXIT_FAILURE);
            A355606_ones = t;
            A355606_cap = nc;
        }
        A355606_ones[A355606_cnt++] = A355606_i;
    }
    size_t dn = (size_t)num_divisors(val);
    A355606_ensure_freq(dn);
    A355606_freq[dn]++;
    A355606_prev = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A355606_cnt < (size_t)n) A355606_step();
    return (seq_t)A355606_ones[(size_t)n - 1];
}
