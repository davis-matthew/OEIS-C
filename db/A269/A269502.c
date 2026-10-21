/*
 * A269502  Number of contiguous substrings of the binary representation of n
 *          that are equal to some a(m) where 1 <= m < n and where a(1)=1.
 * https://oeis.org/A269502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of contiguous binary substrings of n that are
 *            equal to some a(m) with 1 <= m < n (a(1)=1). The sequence is
 *            computed sequentially and cached
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../utils/floor_log2_uint64.h"

static seq_t *values = NULL;     /* cache of a(n) */
static size_t values_cap = 0;
static idx_t max_computed = 0;

static bool *present = NULL;     /* present[v] == true if some a(m)=v with m<n */
static size_t present_cap = 0;

static void ensure_values_cap(size_t needed)
{
    if (needed <= values_cap)
        return;
    size_t new_cap = values_cap ? values_cap << 1 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    memset(tmp + values_cap, 0, (new_cap - values_cap) * sizeof(seq_t));
    values = tmp;
    values_cap = new_cap;
}

static void ensure_present_cap(size_t needed)
{
    if (needed <= present_cap)
        return;
    size_t new_cap = present_cap ? present_cap << 1 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(present, new_cap * sizeof(bool));
    if (!tmp)
        abort();
    memset(tmp + present_cap, 0, (new_cap - present_cap) * sizeof(bool));
    present = tmp;
    present_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined below offset, but safe */
    if (n > max_computed) {
        /* extend cache to cover n */
        ensure_values_cap((size_t)n + 1);
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            seq_t val;
            if (k == 1) {
                val = 1; /* base case */
            } else {
                /* bits of k */
                idx_t tk = k;
                unsigned L = floor_log2_uint64(tk) + 1;
                uint8_t bits[64];
                for (unsigned i = 0; i < L; ++i)
                    bits[i] = (uint8_t)((tk >> (L - 1 - i)) & 1ULL);
                /* count substrings equal to earlier a(m) */
                seq_t count = 0;
                for (unsigned start = 0; start < L; ++start) {
                    seq_t sub = 0;
                    for (unsigned end = start; end < L; ++end) {
                        sub = (sub << 1) | bits[end];
                        if (sub > 0) {
                            size_t idx = (size_t)sub;
                            if (idx < present_cap && present[idx])
                                ++count;
                        }
                    }
                }
                val = count;
            }
            values[(size_t)k] = val;
            /* make sure present array big enough to mark val */
            /* val is expected to fit in size_t for the allowed n */
            size_t val_sz = (size_t)val;
            if (val_sz >= present_cap)
                ensure_present_cap(val_sz + 1);
            present[val_sz] = true;
        }
        max_computed = n;
    }
    return values[(size_t)n];
}
