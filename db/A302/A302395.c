/*
 * A302395  a(n) is the number of ways of writing the binary expansion of n as
 *          a concatenation of distinct nonempty substrings.
 * https://oeis.org/A302395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1046, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   substring_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration of partitions of the binary representation of n into
   distinct non‑empty substrings.  For n=0 the empty concatenation
   counts as one way.  The function examines all 2^(len-1) possible
   cut placements (len = number of bits of n) and counts those
   partitions where no two substrings are identical as strings
   (leading zeros inside a substring are significant).  The binary
   length of any n ≤ 10000 is at most 14, so the search is very
   fast. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/substring_value.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n = 0 => empty string, one way */
    if (n == 0)
        return 1;

    /* Determine the number of significant bits of n */
    int len = 0;
    idx_t tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* For a single bit, only the whole string is possible */
    if (len == 1)
        return 1;

    /* Convert the bits of n to an array MSB→LSB, so that
       substring_value can be used directly. */
    uint8_t bits[64];
    for (int i = 0; i < len; ++i)
        bits[i] = (uint8_t)((n >> (len - 1 - i)) & 1ULL);

    seq_t count = 0;

    /* Enumerate all ways of inserting cuts between adjacent bits.
       There are len-1 possible cut positions, represented by the bits
       of mask.  Mask 0 means no cuts (the whole string). */
    uint64_t total_masks = 1ULL << (len - 1);
    for (uint64_t mask = 0; mask < total_masks; ++mask) {
        int start = 0;            /* start index of current segment */
        bool ok = true;
        seq_t used[64];
        int used_cnt = 0;

        /* Process each cut position */
        for (int i = 0; i < len - 1; ++i) {
            if (mask & (1ULL << i)) {
                /* Segment bits[start .. i] inclusive */
                seq_t val   = substring_value(bits, start, i + 1);
                int  seglen = i + 1 - start;
                /* Encode the segment as (value << 4) | length
                   so that leading zeros are significant. */
                seq_t key = (val << 4) | (seq_t)seglen;

                /* Check for duplicates */
                for (int k = 0; k < used_cnt; ++k) {
                    if (used[k] == key) {
                        ok = false;
                        break;
                    }
                }
                if (!ok)
                    break;

                used[used_cnt++] = key;
                start = i + 1;   /* next segment starts after i */
            }
        }

        if (!ok)
            continue;   /* invalid partition */

        /* Last segment (from current start to end) */
        seq_t val   = substring_value(bits, start, len);
        int  seglen = len - start;
        seq_t key = (val << 4) | (seq_t)seglen;

        for (int k = 0; k < used_cnt; ++k) {
            if (used[k] == key) {
                ok = false;
                break;
            }
        }
        if (ok)
            ++count;
    }

    return count;
}
