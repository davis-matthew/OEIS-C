/*
 * A241672  Numbers k such that A035014(k) begins with a 4.
 * https://oeis.org/A241672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k with A035014(k) beginning with 4; A035014(k) leads
 *            with 4 iff bit k-1 of A035014(k-1) is 0 (A035014(0)=0); computed
 *            by incremental big-integer search
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define K_LIMBS 160
#define K_MAXK 1200

static uint32_t K_A[K_LIMBS];
static uint32_t K_P[K_LIMBS];
static unsigned K_terms[K_MAXK + 2];
static size_t K_cnt = 0;
static unsigned K_m = 1;
static int K_init = 0;

static void K_step(void)
{
    unsigned m = K_m;
    unsigned bit = m - 1;
    int b = (int)((K_A[bit >> 5] >> (bit & 31)) & 1u);
    uint64_t d = b ? 3 : 4;
    if (!b) K_terms[K_cnt++] = m;
    uint64_t carry = 0;
    for (int i = 0; i < K_LIMBS; i++) {
        uint64_t cur = (uint64_t)K_A[i] + (uint64_t)K_P[i] * d + carry;
        K_A[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
    carry = 0;
    for (int i = 0; i < K_LIMBS; i++) {
        uint64_t cur = (uint64_t)K_P[i] * 10u + carry;
        K_P[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
    K_m++;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!K_init) {
        for (int i = 0; i < K_LIMBS; i++) { K_A[i] = 0; K_P[i] = 0; }
        K_P[0] = 1;
        K_init = 1;
    }
    while (K_cnt < (size_t)n && K_m <= K_MAXK)
        K_step();
    return (seq_t)K_terms[n - 1];
}
