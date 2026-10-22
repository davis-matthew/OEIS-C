/*
 * A338829  a(n) is the greatest number not yet in the sequence with the same
 *          number of digits and the same sum of digits as n.
 * https://oeis.org/A338829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A338829: a(n) is the greatest number not yet in the sequence with
   the same number of decimal digits and the same digit sum as n.
   The algorithm pairs numbers within each (digit length, digit sum)
   group: the i-th smallest number is paired with the (count-1-i)-th
   smallest.  For a given n we compute its rank in the group and
   reconstruct the partner at the reflected rank. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* DP table: ways[pos][sum] = number of ways to fill 'pos' digits
   (each 0-9) so that their sum is 'sum'.  pos ranges 0..20, sum 0..180. */
static unsigned __int128 ways[21][181];
static bool ways_inited = false;

static void init_ways(void) {
    if (ways_inited) return;
    ways_inited = true;
    ways[0][0] = 1;
    for (int s = 1; s <= 180; ++s) ways[0][s] = 0;
    for (int pos = 1; pos <= 20; ++pos) {
        int max_sum = 9 * pos;
        for (int s = 0; s <= max_sum; ++s) {
            unsigned __int128 cnt = 0;
            for (int d = 0; d <= 9; ++d) {
                if (d > s) break;
                cnt += ways[pos-1][s-d];
            }
            ways[pos][s] = cnt;
        }
    }
}

/* Extract decimal digits of n into digits[0..len-1] (MSB first). */
static void digits_of_uint64(uint64_t n, uint8_t *digits, size_t *lenp) {
    if (n == 0) {
        digits[0] = 0;
        *lenp = 1;
        return;
    }
    uint8_t tmp[20];
    size_t len = 0;
    while (n > 0) {
        tmp[len++] = n % 10;
        n /= 10;
    }
    for (size_t i = 0; i < len; ++i) {
        digits[i] = tmp[len-1-i];
    }
    *lenp = len;
}

/* Number of numbers with length L and digit sum S. Leading zero is
   forbidden for L > 1. */
static unsigned __int128 count_total(idx_t L, idx_t S) {
    unsigned __int128 total = 0;
    if (L == 1) {
        if (S <= 9) total = 1;
    } else {
        for (int d = 1; d <= 9; ++d) {
            if ((idx_t)d <= S) {
                total += ways[L-1][S-d];
            }
        }
    }
    return total;
}

/* Rank of the number (represented by its digits array) among numbers
   with the same length and digit sum (0-based). */
static unsigned __int128 rank_of(idx_t L, idx_t S, const uint8_t *digits) {
    uint8_t sum_left = (uint8_t)S;
    unsigned __int128 rank = 0;
    for (size_t pos = 0; pos < L; ++pos) {
        int cur = digits[pos];
        int min_d = (pos == 0 && L > 1) ? 1 : 0;
        for (int d = min_d; d < cur; ++d) {
            if (sum_left < (uint8_t)d) break;
            rank += ways[L - pos - 1][sum_left - d];
        }
        sum_left -= cur;
    }
    return rank;
}

/* Reconstruct the number corresponding to the given rank within the
   group defined by L and S. */
static seq_t number_from_rank(idx_t L, idx_t S, unsigned __int128 target_rank) {
    unsigned __int128 rank = target_rank;
    unsigned __int128 val = 0;
    uint8_t sum_left = (uint8_t)S;
    for (size_t pos = 0; pos < L; ++pos) {
        int rem = (int)(L - pos - 1);
        int min_d = (pos == 0 && L > 1) ? 1 : 0;
        int chosen = min_d;
        for (int d = min_d; d <= 9; ++d) {
            if (sum_left < (uint8_t)d) break;
            unsigned __int128 cnt = ways[rem][sum_left - d];
            if (cnt > rank) {
                chosen = d;
                break;
            } else {
                rank -= cnt;
            }
        }
        val = val * 10 + chosen;
        sum_left -= chosen;
    }
    return (seq_t)val;
}

seq_t a(idx_t n) {
    init_ways();

    uint8_t dig[20];
    size_t len;
    digits_of_uint64((uint64_t)n, dig, &len);
    idx_t L = len;

    idx_t S = 0;
    for (size_t i = 0; i < len; ++i) S += dig[i];

    unsigned __int128 cnt   = count_total(L, S);
    unsigned __int128 r     = rank_of(L, S, dig);
    unsigned __int128 target = cnt - 1u - r;

    return number_from_rank(L, S, target);
}
