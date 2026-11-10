/*
 * A317726  Length of runs of terms in A104381 which differ only in the last
 *          decimal place.
 * https://oeis.org/A317726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of terms k of A104381 (10^(k-1) == 1 mod k) in the
 *            n-th nonempty decade [10m, 10m+9]; computed by search
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A317726_is_good(uint64_t k)
{
    if (k == 1) return true;
    uint64_t res = 1, base = 10 % k, e = k - 1;
    while (e) {
        if (e & 1) res = (uint64_t)(((unsigned __int128)res * base) % k);
        e >>= 1;
        if (e) base = (uint64_t)(((unsigned __int128)base * base) % k);
    }
    return res == 1;
}

seq_t a(idx_t n)
{
    static seq_t *runs = NULL;
    static size_t size = 0, cap = 0;
    static uint64_t decade = 0;
    static const int digs[4] = {1, 3, 7, 9};

    while (size < (size_t)n) {
        unsigned cnt = 0;
        for (int i = 0; i < 4; i++) {
            uint64_t k = decade * 10 + (uint64_t)digs[i];
            if (A317726_is_good(k)) cnt++;
        }
        decade++;
        if (cnt == 0) continue;
        if (size >= cap) {
            if (cap == 0) {
                cap = 1024;
                runs = mem_new(seq_t, cap);
            } else {
                cap *= 2;
                runs = mem_resize(seq_t, runs, cap);
            }
        }
        runs[size++] = (seq_t)cnt;
    }
    return runs[n - 1];
}
