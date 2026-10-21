/*
 * A250207  The number of quartic terms in the multiplicative group modulo n.
 * https://oeis.org/A250207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = φ(n) / A073103(n)
 * Range:     u8 n <= 522, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

static seq_t prime_power_value(idx_t p, idx_t e)
{
    if (p == 2) {
        if (e <= 3)
            return (seq_t)1;
        return pow_seq((seq_t)2, (int)(e - 4));
    }
    seq_t base = (seq_t)p;
    seq_t pw = pow_seq(base, (int)(e - 1));
    seq_t mult = pw * (base - (seq_t)1);
    if (p % 4 == 1)
        return mult / (seq_t)4;
    return mult / (seq_t)2;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = (idx_t)pr[i];
        idx_t e = (idx_t)ex[i];
        result *= prime_power_value(p, e);
    }
    return result;
}
