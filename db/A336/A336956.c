/*
 * A336956  For any number n whose set of nonzero decimal digits is { d_0,
 *          ..., d_k } (with d_0 < ... < d_k), a(n) is obtained by replacing
 *          in the decimal representation of n each nonzero digit d_m by
 *          d_{k-m} for m = 0..k.
 * https://oeis.org/A336956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by replacing each nonzero digit d of n by its *
 *            mirror in the sorted set of the nonzero decimal digits present
 *            in n
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: n = 0 */
    if (n == 0)
        return 0;

    /* Extract decimal digits (least significant first). 20 digits are enough
       for all 64‑bit integers (18,446,744,073,709,551,615 has 20 digits). */
    uint8_t digits[20];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Detect which non‑zero digits are present. */
    bool has[10] = { false };
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = digits[i];
        if (d != 0)
            has[d] = true;
    }

    /* Build the sorted list of distinct non‑zero digits. */
    int distinct[9];
    size_t k = 0;
    for (int d = 1; d <= 9; ++d)
        if (has[d]) distinct[k++] = d;

    /* Compute the mapping: each digit d maps to distinct[k‑1‑idx] where
       idx is the index of d in the distinct array. */
    int mapping[10] = { 0 };      /* mapping[0] stays 0 */
    for (size_t idx = 0; idx < k; ++idx) {
        int d = distinct[idx];
        int mapped = distinct[k - 1 - idx];
        mapping[d] = mapped;
    }

    /* Reconstruct the result from most significant to least. */
    seq_t res = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        int d = digits[i];
        int d2 = mapping[d];
        res = res * (seq_t)10 + (seq_t)d2;
    }
    return res;
}
