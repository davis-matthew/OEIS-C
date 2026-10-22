/*
 * A375403  Numbers whose maximal anti-runs of weakly increasing prime factors
 *          (with multiplicity) do not have distinct maxima.
 * https://oeis.org/A375403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer whose largest prime divisor
 *            occurs at least twice in its factorisation (in other words,
 *            numbers divisible by p^2 where p is the maximal prime factor, or
 *            numbers having some other prime repeated at least three times)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static bool qualifies(uint64_t x) {
    uint64_t temp = x;
    uint64_t largest_p = 0, largest_exp = 0;
    bool any_ge3 = false;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            if (cnt >= 3) any_ge3 = true;
            if (p > largest_p) {
                largest_p = p;
                largest_exp = cnt;
            }
        }
    }
    if (temp > 1) {                 /* temp is a prime > 1 */
        if (temp > largest_p) {
            largest_p = temp;
            largest_exp = 1;
        }
    }
    if (largest_exp >= 2) return true;
    return any_ge3;
}

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t checked = 2;
    while (seq_len < n) {
        if (qualifies(checked)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)checked;
        }
        ++checked;
    }
    return seq[n - 1];
}
