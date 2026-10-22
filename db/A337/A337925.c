/*
 * A337925  Digits of n rearranged to be the smallest number with the fewest
 *          possible prime factors, counted with multiplicity. Terms retain
 *          the same number of digits as n, i.e. leading digits may not be
 *          zero.
 * https://oeis.org/A337925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   compute a(n) by searching over digit permutations. For each n >=
 *            1, find the permutation of its decimal digits (with no leading
 *            zeros) that has the smallest number of prime factors counted
 *            with multiplicity. Among ties pick the smallest numeric value.
 *            The function is called many times, so an effi...
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_uint8, count_prime_factors, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Utility headers. */
#include "../utils/get_digits.h"
#include "../utils/count_prime_factors.h"
#include "../utils/cmp_uint8.h"

/* Recursive permutation generator. */
static void permute_rec(size_t pos,
                        seq_t cur,
                        bool *used,
                        size_t len,
                        uint8_t *digits,
                        seq_t *best_val,
                        seq_t *best_fcnt,
                        bool *best_set)
{
    if (pos == len) {
        seq_t fcnt = count_prime_factors(cur);
        if (!*best_set ||
            fcnt < *best_fcnt ||
            (fcnt == *best_fcnt && cur < *best_val))
        {
            *best_set = true;
            *best_fcnt = fcnt;
            *best_val  = cur;
        }
        return;
    }

    for (size_t i = 0; i < len; ++i) {
        if (used[i]) continue;
        if (pos == 0 && digits[i] == 0) continue;         /* no leading zero */
        if (i > 0 && digits[i] == digits[i - 1] && !used[i - 1])
            continue;                                      /* skip duplicates */

        used[i] = true;
        seq_t next = cur * 10 + (seq_t)digits[i];
        permute_rec(pos + 1, next, used, len, digits,
                    best_val, best_fcnt, best_set);
        used[i] = false;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Single digit numbers are fixed. */
    if (n < 10) return (seq_t)n;

    seq_t sn = (seq_t)n;
    size_t len;
    uint8_t raw_digits[64];

    /* Extract digits of n. */
    get_digits(sn, raw_digits, &len);

    /* Sort digits to enable duplicate pruning. */
    uint8_t digits[64];
    memcpy(digits, raw_digits, len * sizeof(uint8_t));
    qsort(digits, len, sizeof(uint8_t), cmp_uint8);

    bool used[64] = { false };
    seq_t best_val = 0;
    seq_t best_fcnt = 0;
    bool best_set = false;

    /* Generate all valid permutations. */
    permute_rec(0, (seq_t)0, used, len, digits,
                &best_val, &best_fcnt, &best_set);

    return best_val;
}
