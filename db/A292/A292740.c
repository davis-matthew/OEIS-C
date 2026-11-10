/*
 * A292740  Indices k such that A292547(k) = 0.
 * https://oeis.org/A292740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 that is not a sum of distinct odd cubes
 *            (A292547(k)=0); computed by a subset-sum sieve up to 300000
 *            (conjecturally no terms beyond 212594)
 * Range:     u8 n <= 248, u16 n <= 44414, u32 n <= 54766, u64 n <= 54766,
 *            u128 n <= 54766, big any n
 * Validated: standard, pass, 83 terms through n = 9753 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

#define A292740_LIMIT 300000

static uint32_t A292740_terms[A292740_LIMIT + 1];
static unsigned A292740_count = 0;
static int A292740_ready = 0;

static void A292740_init(void)
{
    static unsigned char reach[A292740_LIMIT + 1];
    memset(reach, 0, sizeof(reach));
    reach[0] = 1;
    for (uint32_t odd = 1; ; odd += 2) {
        uint64_t cube = (uint64_t)odd * odd * odd;
        if (cube > A292740_LIMIT) break;
        for (uint32_t s = A292740_LIMIT; s >= cube; --s) {
            if (reach[s - cube]) reach[s] = 1;
        }
    }
    A292740_count = 0;
    for (uint32_t k = 1; k <= A292740_LIMIT; ++k) {
        if (!reach[k]) A292740_terms[A292740_count++] = k;
    }
    A292740_ready = 1;
}

seq_t a(idx_t n)
{
    if (!A292740_ready) A292740_init();
    if (n < 1 || n > A292740_count) return (seq_t)0;
    return (seq_t)A292740_terms[n - 1];
}
