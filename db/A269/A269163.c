/*
 * A269163  Numbers which have a finite predecessor in Wolfram's Rule 30
 *          cellular automaton; range of A269160 sorted into ascending order.
 * https://oeis.org/A269163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   bitlen_seq_t, can_extend_with_zeros, rule_output
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = nth integer that has a finite predecessor in Wolfram's
 * Rule 30 cellular automaton.  Equivalent to: the binary representation
 * of a(n) can be produced from a finite configuration under Rule 30.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/can_extend_with_zeros.h"
#include "../utils/rule_output.h"

/* Returns true iff ‘value’ has a finite predecessor in Rule 30. */
static bool has_predecessor(seq_t value)
{
    if (value == (seq_t)0)
        return true;                /* the only number with itself as predecessor */

    idx_t len = bitlen_seq_t(value);   /* number of bits in value (>0) */
    if (len == 0)                       /* should not happen but be safe */
        return true;

    unsigned cur_mask = 0;

    /* first output bit (most significant) */
    int first_bit = (int)((value >> (len - 1)) & (seq_t)1);

    /* left boundary: two zeros preceding the first output.  This forces
     * rule(0,0,p0)==0, i.e., p0 must be 0.  That forbids the leading
     * constant 1 of the predecessor. */
    for (unsigned p0 = 0; p0 <= 1; ++p0) {
        for (unsigned p1 = 0; p1 <= 1; ++p1) {
            if (rule_output(0, p0, p1) == (unsigned)first_bit &&
                rule_output(0, 0, p0) == 0) {
                cur_mask |= 1U << ((p0 << 1) | p1);
            }
        }
    }
    if (cur_mask == 0)
        return false;

    /* remaining output bits */
    for (idx_t pos = 1; pos < len; ++pos) {
        int bit = (int)((value >> (len - 1 - pos)) & (seq_t)1);
        unsigned next_mask = 0;
        for (unsigned s = 0; s < 4; ++s) {
            if (cur_mask & (1U << s)) {
                unsigned p0 = (s >> 1) & 1;
                unsigned p1 = s & 1;
                for (unsigned p2 = 0; p2 <= 1; ++p2) {
                    if (rule_output(p0, p1, p2) == (unsigned)bit) {
                        unsigned ns = (p1 << 1) | p2;
                        next_mask |= 1U << ns;
                    }
                }
            }
        }
        cur_mask = next_mask;
        if (cur_mask == 0)
            return false;
    }

    /* right boundary: must be able to extend with zeros to the right */
    for (unsigned s = 0; s < 4; ++s) {
        if (cur_mask & (1U << s)) {
            if (can_extend_with_zeros(s))
                return true;
        }
    }
    return false;
}

/* ---------- Sequence access function ------------------------------------ */

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 0;

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = (seq_t *)tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    grow_cache(n + 1);
    while (cache_len <= n) {
        seq_t cand = (seq_t)next_candidate;
        if (has_predecessor(cand))
            cache[cache_len++] = cand;
        ++next_candidate;
    }
    return cache[n];
}
