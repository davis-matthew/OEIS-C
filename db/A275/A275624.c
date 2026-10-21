/*
 * A275624  a(n) = minimal m such that Sum_{k=1..m} 0.k >= n.
 * https://oeis.org/A275624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal m such that \sum_{k=1}^{m} k/10^{digits(k)} >= n
 * Range:     u8 n <= 5, u16 n <= 229, u32 n <= 9133, u64 n <= 9133,
 *            u128 n <= 9133, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

static idx_t digits(idx_t x) {
    if (x < 10ULL)          return 1;
    if (x < 100ULL)         return 2;
    if (x < 1000ULL)        return 3;
    if (x < 10000ULL)       return 4;
    if (x < 100000ULL)      return 5;
    if (x < 1000000ULL)     return 6;
    if (x < 10000000ULL)    return 7;
    if (x < 100000000ULL)   return 8;
    if (x < 1000000000ULL)  return 9;
    if (x < 10000000000ULL) return 10;
    if (x < 100000000000ULL)      return 11;
    if (x < 1000000000000ULL)     return 12;
    if (x < 10000000000000ULL)    return 13;
    if (x < 100000000000000ULL)   return 14;
    if (x < 1000000000000000ULL)  return 15;
    if (x < 10000000000000000ULL) return 16;
    if (x < 100000000000000000ULL) return 17;
    return 18; /* covers up to 64‑bit maximum */
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, but guard against zero */
    seq_t sum_num = (seq_t)0;
    idx_t denom_exp = 0; /* current exponent of 10 in denominator */
    for (idx_t k = 1; ; ++k) {
        idx_t d = digits(k);
        if (d > denom_exp) {
            sum_num *= pow10(d - denom_exp);
            denom_exp = d;
        }
        seq_t threshold = (seq_t)n * pow10(denom_exp);
        sum_num += k; /* add k / 10^d, but after scaling denom=10^d */
        if (sum_num >= threshold) {
            return (seq_t)k;
        }
    }
}
