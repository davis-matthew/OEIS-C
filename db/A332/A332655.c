/*
 * A332655  a(n) = Sum_{k=1..n} (k/gcd(n, k))^n.
 * https://oeis.org/A332655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k/gcd(n, k))^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t sn = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t g = gcd(sn, kseq);
        seq_t base = kseq / g;
        seq_t term = big_pow(base, sn);
        sum += term;
    }
    return sum;
}
