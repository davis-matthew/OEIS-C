/*
 * A324495  Average number of steps t(n) required to get n by repeatedly
 *          toggling one of the ceiling(log_2(n)) bits of the binary result of
 *          the previous step at a random position with equal probability of
 *          the bit positions, starting with all bits 0. The fractional part
 *          of t is given separately, i.e., t(n) =...
 * https://oeis.org/A324495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 326, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ⌊ expected number of steps to reach n in the
 * bit‑toggle process (average over random toggles) ⌋   (OEIS A324495)
 */
#include "../utils/gcd.h"

static inline idx_t bitlen(idx_t v) {
    /* number of bits needed to represent v (v >= 1) */
    return 64 - __builtin_clzll(v);
}

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t m = bitlen(n);                /* number of bits of n */
    idx_t w = __builtin_popcountll(n);  /* Hamming weight of n */

    /* D[1..m] : gaps between successive expected lengths as reduced fractions */
    static struct { seq_t num, den; } D[65];

    if (m == 1) {
        D[1].num = 1;
        D[1].den = 1;
    } else {
        /* initialise the last two elements */
        D[m].num = 1;
        D[m].den = 1;                   /* D_m = 1/1 */
        D[m-1].num = (seq_t)(m + 1);
        D[m-1].den = (seq_t)(m - 1);
        seq_t g = gcd(D[m-1].num, D[m-1].den);
        D[m-1].num /= g;
        D[m-1].den /= g;

        for (idx_t k = m - 2; k >= 1; --k) {
            /* D_k = (m * D_{k+1}.den + (m-k) * D_{k+1}.num) /
                     (k * D_{k+1}.den)  , reduced */
            seq_t num_new = (seq_t)m * D[k+1].den + (seq_t)(m - k) * D[k+1].num;
            seq_t den_new = (seq_t)k * D[k+1].den;
            g = gcd(num_new, den_new);
            D[k].num = num_new / g;
            D[k].den = den_new / g;
        }
    }

    /* Sum the first w gaps to obtain the expected number of steps */
    seq_t sum_num = 0;
    seq_t sum_den = 1;
    for (idx_t i = 1; i <= w; ++i) {
        seq_t a_num = D[i].num;
        seq_t a_den = D[i].den;
        seq_t new_num = sum_num * a_den + a_num * sum_den;
        seq_t new_den = sum_den * a_den;
        seq_t g = gcd(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;
    }

    /* Return the integer part of the expected value */
    return sum_num / sum_den;
}
