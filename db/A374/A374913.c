/*
 * A374913  Numbers k such that k^(k + 1) == k + 1 (mod 2*k + 1).
 * https://oeis.org/A374913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer k with k^(k+1) ≡ k+1 (mod
 *            2*k+1)
 * Range:     u8 n <= 28, u16 n <= 6136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9766 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;       /* array of found k values   */
static size_t cache_cap = 0;      /* current allocated size    */
static size_t cache_size = 0;     /* number of values stored   */
static idx_t   next_k   = 1;      /* next k to test            */

static void add_value(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */
    while (cache_size < n) {
        idx_t k = next_k++;
        seq_t kseq = (seq_t)k;
        seq_t mod = kseq * 2 + 1;
        seq_t pow = powmod(kseq, k + 1, mod);
        if (pow == kseq + 1) {
            add_value(kseq);
        }
    }
    return cache[n - 1];
}
