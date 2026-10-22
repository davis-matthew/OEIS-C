/*
 * A338831  a(n) is the greatest number not yet in the sequence with the same
 *          number of digits and the same smallest digit as n.
 * https://oeis.org/A338831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the greatest unused number that has the same decimal *
 *            length and the same smallest digit as n. The sequence is a *
 *            self‑inverse permutation of the nonnegative integers. The *
 *            implementation computes, for each input n, the rank of n inside
 *            * the group of numbers with equal (length, smalle...
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   bucket_size, compute_rank, count_min_geq, get_value, pow_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/bucket_size.h"
#include "../utils/compute_rank.h"
#include "../utils/count_min_geq.h"
#include "../utils/get_value.h"
#include "../utils/pow_ull.h"

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Extract decimal digits of n (least significant first). */
    uint8_t digits[64];
    idx_t d = 0;
    seq_t x = (seq_t)n;

    if (x == 0) {
        digits[d++] = 0;
    } else {
        while (x > 0) {
            digits[d++] = (uint8_t)(x % 10);
            x /= 10;
        }
    }

    /* One‑digit numbers map to themselves. */
    if (d == 1) return (seq_t)n;

    /* Smallest digit in n. */
    unsigned char s = 9;
    for (idx_t i = 0; i < d; ++i) {
        if (digits[i] < s) s = digits[i];
    }

    /* Size of the group. */
    idx_t sz = bucket_size(d, s);

    /* Rank of n within its group. */
    idx_t rnk = compute_rank(n, d, s, digits);

    /* Complementary rank for the paired number. */
    idx_t target = sz - 1 - rnk;

    /* Construct the number with that rank. */
    return get_value(d, s, target);
}
