/*
 * A300950  Fixed points of A300948.
 * https://oeis.org/A300950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..k} b_i * p(i+1) for n = Sum b_i*2^i, where p(m)
 *            is the m-th smallest primitive fixed point of A300948: let
 *            s(0)=1, s(1)=0, s(i)=Sum_{j in bits(i+1)} 2^s(j) - 1 (an
 *            involution); p's are 2^M + (s(M)<M ? 2^s(M) : 0) for M = 1,2,...
 *            with s(M) <= M
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A300950_sg(uint64_t i)
{
    if (i == 0) return 1;
    if (i == 1) return 0;
    uint64_t x = i + 1, s = 0, j = 0;
    while (x) {
        if (x & 1) s += ((uint64_t)1 << A300950_sg(j));
        x >>= 1;
        ++j;
    }
    return s - 1;
}

static uint64_t A300950_M[70];
static bool A300950_init = false;

static void A300950_setup(void)
{
    if (A300950_init) return;
    int cnt = 0;
    uint64_t M = 1;
    while (cnt < 66) {
        if (A300950_sg(M) <= M) A300950_M[cnt++] = M;
        ++M;
    }
    A300950_init = true;
}

static seq_t A300950_pow2(uint64_t e)
{
    seq_t r = (seq_t)1;
    for (uint64_t i = 0; i < e; ++i) r = r * (seq_t)2;
    return r;
}

seq_t a(idx_t n)
{
    A300950_setup();
    seq_t sum = (seq_t)0;
    int i = 0;
    while (n) {
        if (n & 1) {
            uint64_t M = A300950_M[i];
            uint64_t s = A300950_sg(M);
            sum += A300950_pow2(M);
            if (s < M) sum += A300950_pow2(s);
        }
        n >>= 1;
        ++i;
    }
    return sum;
}
