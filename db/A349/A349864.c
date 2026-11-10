/*
 * A349864  Smallest k such that A349278(k) = n, or 0 if no such k exists.
 * https://oeis.org/A349864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest k with A349278(k) = n, else 0; found by DFS over
 *            digits from the least significant, where each running digit-sum
 *            must divide the remaining product
 * Range:     u8 n <= 23, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void A349864_dfs(uint64_t s, uint64_t r, seq_t k, seq_t place, seq_t *best, bool *found)
{
    for (unsigned d = 0; d <= 9; d++) {
        uint64_t sp = s + d;
        if (sp == 0) continue;
        if (d == 0 && s <= 1) continue;      /* zero digit with sum 1 never helps */
        if (r % sp != 0) continue;
        uint64_t nr = r / sp;
        seq_t nk = k + (seq_t)d * place;
        if (nr == 1) {
            if (d != 0) {
                if (!*found || nk < *best) {
                    *best = nk;
                    *found = true;
                }
            }
        } else {
            A349864_dfs(sp, nr, nk, place * 10, best, found);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t best = 0;
    bool found = false;
    A349864_dfs(0, (uint64_t)n, (seq_t)0, (seq_t)1, &best, &found);
    return found ? best : (seq_t)0;
}
