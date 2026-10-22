/*
 * A368070  a(n) is the number of sequences of binary words (w_1, ..., w_k)
 *          such that w_1 corresponds to the binary expansion of n (without
 *          leading zeros), for m = 1..k-1, w_{m+1} is obtained by removing
 *          one bit from w_m, and w_k is the empty word.
 * https://oeis.org/A368070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   number of ways to delete the binary representation of n bit by
 *            bit; a recursive DP over all distinct intermediate strings
 * Range:     u8 n <= 41, u16 n <= 552, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_LEN 15                    /* maximum string length needed (n <= 10000 ⇒ len <= 14) */
#define KEY_BITS (1 << 14)            /* mask width for length up to 14 bits */
#define KEY_SIZE ((MAX_LEN + 1) * KEY_BITS)

static seq_t memo[KEY_SIZE];
static bool seen[KEY_SIZE] = {0};

/* Recursive helper: number of sequences for a binary string represented
   by `mask` (len bits, least significant bit is first character). */
static seq_t dp(uint64_t mask, uint8_t len)
{
    if (len == 0)
        return (seq_t)1;               /* empty word gives one empty sequence */

    size_t key = (((size_t)len) << 14) | (size_t)mask;
    if (seen[key])
        return memo[key];

    /* generate all distinct children by deleting one bit and sum their counts */
    seq_t res = (seq_t)0;
    uint64_t child_masks[16];
    uint8_t child_cnt = 0;

    for (uint8_t i = 0; i < len; ++i) {
        /* Remove bit i */
        uint64_t left  = mask & ((1ULL << i) - 1ULL);
        uint64_t right = mask >> (i + 1);
        uint64_t child = left | (right << i);

        /* Deduplicate identical children */
        bool dup = false;
        for (uint8_t j = 0; j < child_cnt; ++j)
            if (child_masks[j] == child) { dup = true; break; }
        if (dup) continue;

        child_masks[child_cnt++] = child;
        res += dp(child, len - 1);
    }

    seen[key] = true;
    memo[key] = res;
    return res;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* a(0) = 1 by definition */

    /* determine binary length */
    uint64_t tmp = (uint64_t)n;
    uint8_t len = 0;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    uint64_t mask = (uint64_t)n;       /* binary representation fits in mask */
    return dp(mask, len);
}
