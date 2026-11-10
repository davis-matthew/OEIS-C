/*
 * A335200  Positions of difference 12 in A070195.
 * https://oeis.org/A335200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A070195(k+1) - A070195(k) = 12; found by
 *            search over A070195 (6m, m squarefree, coprime to 6, 6m-1 and
 *            6m+1 both prime)
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-08)
 * Helpers:   is_prime, is_squarefree, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_squarefree.h"
#include "../utils/mem.h"

static seq_t A070195_a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t size = 0, cap = 0;
    static idx_t next_m = 1;

    if (n == 0) return 0;
    if (size == 0 && cache == NULL) {
        cap = 128;
        cache = mem_new(seq_t, cap);
    }
    while (size < n) {
        while ((next_m & 1ULL) == 0 || (next_m % 3 == 0))
            ++next_m;
        idx_t p  = 6 * next_m - 1;
        idx_t p2 = 6 * next_m + 1;
        if (is_prime(p) && is_prime(p2)) {
            if (is_squarefree((seq_t)next_m)) {
                seq_t val = (seq_t)6 * (seq_t)next_m;
                if (size == cap) {
                    cap <<= 1;
                    cache = mem_resize(seq_t, cache, cap);
                }
                cache[size++] = val;
            }
        }
        ++next_m;
    }
    return cache[n - 1];
}

seq_t a(idx_t n)
{
    static seq_t *pos = NULL;
    static size_t found = 0, pcap = 0;
    static idx_t k = 0;

    if (n == 0) return 0;
    if (pos == NULL) {
        pcap = 64;
        pos = mem_new(seq_t, pcap);
    }
    while (found < n) {
        ++k;
        seq_t x = A070195_a(k);
        seq_t y = A070195_a(k + 1);
        if (y - x == 12) {
            if (found == pcap) {
                pcap <<= 1;
                pos = mem_resize(seq_t, pos, pcap);
            }
            pos[found++] = (seq_t)k;
        }
    }
    return pos[n - 1];
}
