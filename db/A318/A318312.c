/*
 * A318312  Multiplicative with a(p^e) = 2^A000041(e).
 * https://oeis.org/A318312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 2^A000041(n) (multiplicative with a(p^e)=2^p(e))
 * Range:     u8 n <= 63, u16 n <= 255, u32 n <= 1023, u64 n <= 4095,
 *            u128 n <= 8979, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* pre‑compute partition numbers p(e)=A000041(e) for e≤64 */
static uint32_t part_tbl[65];
static bool part_inited = false;

static void init_partitions(void)
{
    if (part_inited) return;
    part_tbl[0] = 1;
    for (uint32_t n = 1; n < 65; ++n) {
        int64_t s = 0;
        for (int k = 1; ; ++k) {
            uint32_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n) break;
            int sign = (k & 1) ? 1 : -1;
            s += sign * (int64_t)part_tbl[n - pent1];
            uint32_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > n) continue;
            s += sign * (int64_t)part_tbl[n - pent2];
        }
        part_tbl[n] = (uint32_t)s;
    }
    part_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* a(1)=1 by definition */

    init_partitions();

    seq_t result = (seq_t)1;

    /* factor n into primes and exponents (assumes n fits in 64 bits) */
    uint64_t primes_buf[32];
    uint64_t exps_buf[32];
    size_t cnt;
    factor_u64(n, primes_buf, exps_buf, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        unsigned e = (unsigned)exps_buf[i];
        seq_t term = big_pow((seq_t)2, (seq_t)part_tbl[e]);
        result *= term;
    }

    return result;
}
