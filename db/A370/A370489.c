/*
 * A370489  Deterministic complexity of binary strings, in shortlex order.
 * https://oeis.org/A370489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = min_{0 <= l < |w|} ( l + minimal_period_of_suffix( w[l..] ) ),
 * where w[n] is the n-th binary string in shortlex order.
 * This implements the deterministic‑complexity computation by searching
 * all suffixes for the smallest repeating period.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for offset < 1 */

    /* Determine the length L of the binary string w[n] */
    idx_t y = n + 2;                           /* y > 0 */
    int floor_log = 63 - __builtin_clzll(y);    /* floor(log2(y)) */
    idx_t pow_floor = ((idx_t)1 << floor_log);  /* 2^floor_log */
    bool is_pow2 = (pow_floor == y);
    int ceil_log = is_pow2 ? floor_log : floor_log + 1; /* ceil(log2(y)) */
    size_t L = (size_t)(ceil_log - 1);          /* length of w */

    /* Compute the integer value of the binary string */
    idx_t start = ((idx_t)1 << L) - 1;          /* index of first string of length L */
    idx_t val = n - start;                     /* 0 <= val < 2^L */

    /* Build the bit array: bits[0] is the most significant bit */
    unsigned char bits[64];
    for (size_t i = 0; i < L; ++i) {
        bits[i] = (unsigned char)((val >> (L - 1 - i)) & 1);
    }

    /* Search over all suffixes to find minimal l + period length */
    seq_t best = (seq_t)(L + 1);                /* upper bound (l = L-1, p = 1) */
    for (size_t l = 0; l < L; ++l) {
        size_t len = L - l;
        size_t p;
        for (p = 1; p <= len; ++p) {
            bool ok = true;
            for (size_t i = 0; i < len; ++i) {
                if (bits[l + i] != bits[l + i % p]) {
                    ok = false;
                    break;
                }
            }
            if (ok)
                break;
        }
        seq_t cand = (seq_t)l + (seq_t)p;
        if (cand < best)
            best = cand;
    }

    return best;
}
