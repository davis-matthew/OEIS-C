/*
 * A330563  Primes in A330562.
 * https://oeis.org/A330563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th prime p such that p is in A330562 (search: test
 *            integers in increasing order, cached)
 * Range:     u8 n <= 3, u16 n <= 333, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 49 terms through n = 9940 (2026-11-08)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_seq.h"

static bool A330563_valid(seq_t k)
{
    bool present[10] = {0,0,0,0,0,0,0,0,0,0};
    seq_t t = k;
    while (t > 0) {
        present[(int)(t % 10)] = true;
        t /= 10;
    }
    if (!present[0]) return false;
    for (int d = 1; d <= 9; ++d) {
        if (present[d]) {
            int r = (int)(k % (seq_t)d);
            if (!present[r]) return false;
        }
    }
    return true;
}

static seq_t *A330563_cache = 0;
static size_t A330563_cnt = 0, A330563_cap = 0;
static seq_t A330563_cur = 100;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A330563_cnt < (size_t)n) {
        A330563_cur++;
        seq_t k = A330563_cur;
        int last = (int)(k % 10);
        if (last != 1 && last != 3 && last != 7 && last != 9) continue;
        if (!A330563_valid(k)) continue;
        if (!is_prime_seq(k)) continue;
        if (A330563_cnt == A330563_cap) {
            size_t nc = A330563_cap ? A330563_cap * 2 : 1024;
            if (A330563_cache == 0) A330563_cache = mem_new(seq_t, nc);
            else A330563_cache = mem_resize(seq_t, A330563_cache, nc);
            A330563_cap = nc;
        }
        A330563_cache[A330563_cnt++] = k;
    }
    return A330563_cache[(size_t)n - 1];
}
