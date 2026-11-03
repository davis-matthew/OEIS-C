/*
 * A368414  Number of factorizations of n into positive integers > 1 such that
 *          it is possible to choose a different prime factor of each factor.
 * https://oeis.org/A368414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001055(n) - A368413(n)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factorize.h"

/* Match a factorization if distinct primes can be assigned to each factor */
static bool match_rec(int i, int depth, uint8_t *mask, uint8_t used, int r)
{
    if (i == depth) return true;
    uint8_t avail = mask[i];
    for (int p = 0; p < r; ++p) {
        if ((avail & (1u << p)) && !(used & (1u << p))) {
            if (match_rec(i + 1, depth, mask, used | (1u << p), r)) return true;
        }
    }
    return false;
}

/* Recursive generation of multiplicative partitions */
static void dfs_factor(idx_t rem, idx_t last, int depth,
                       idx_t *parts, uint8_t *mask,
                       uint64_t *primes, int r,
                       idx_t *divs, size_t divcnt,
                       seq_t *cnt)
{
    if (rem == 1) {
        if (depth > r) return;
        for (int i = 0; i < depth; ++i) {
            uint8_t m = 0;
            idx_t v = parts[i];
            for (int j = 0; j < r; ++j)
                if (v % primes[j] == 0)
                    m |= (1u << j);
            mask[i] = m;
        }
        if (match_rec(0, depth, mask, 0, r)) (*cnt)++;
        return;
    }

    for (size_t k = 0; k < divcnt; ++k) {
        idx_t d = divs[k];
        if (d < last) continue;
        if (d > rem) continue;
        if (rem % d == 0) {
            parts[depth] = d;
            dfs_factor(rem / d, d, depth + 1,
                       parts, mask, primes, r,
                       divs, divcnt, cnt);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Prime factorisation of n */
    uint64_t primes[32];
    int     exps[32];
    int r = (int)factorize((uint64_t)n, primes, exps);
    if (r == 0) return (seq_t)1;   /* Should not happen for n>1 */

    /* Build all divisors of n (excluding 1) */
    idx_t divs[128];
    size_t divcnt = 0;
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            divs[divcnt++] = d;
            if (d != n / d) divs[divcnt++] = n / d;
        }
    }
    divs[divcnt++] = n;  /* n itself */

    /* Simple bubble sort */
    for (size_t i = 0; i + 1 < divcnt; ++i)
        for (size_t j = 0; j + 1 < divcnt - i; ++j)
            if (divs[j] > divs[j + 1]) {
                idx_t t = divs[j];
                divs[j] = divs[j + 1];
                divs[j + 1] = t;
            }

    idx_t  parts[64];
    uint8_t masks[64];
    seq_t  count = 0;

    dfs_factor(n, 2, 0, parts, masks, primes, r,
               divs, divcnt, &count);

    return count;
}
