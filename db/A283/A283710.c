/*
 * A283710  The smallest square referenced in A124809.
 * https://oeis.org/A283710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest square p^2 (p prime) dividing A124809(n), where
 *            A124809 lists the non-squarefree numbers k^2+1; found by
 *            searching k = 1, 2, ... and trial-dividing k^2+1 by primes p ==
 *            1 (mod 4) up to its cube root, then checking whether the
 *            cofactor is a square
 * Range:     u8 n <= 3, u16 n <= 216, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 922 (2026-11-08)
 * Helpers:   icbrt_u64, is_prime_u64, is_square_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/is_square_u64.h"
#include "../utils/icbrt_u64.h"

static uint64_t *res_sq = NULL;
static size_t res_cnt = 0, res_cap = 0;
static uint64_t next_k = 1;

static uint64_t *pr = NULL;
static size_t pr_cnt = 0, pr_cap = 0;
static uint64_t pr_next = 5;

static void ensure_primes(uint64_t lim)
{
    while (pr_next <= lim) {
        if (is_prime_u64(pr_next)) {
            if (pr_cnt == pr_cap) {
                pr_cap = pr_cap ? pr_cap * 2 : 256;
                pr = (uint64_t *)realloc(pr, pr_cap * sizeof(uint64_t));
            }
            pr[pr_cnt++] = pr_next;
        }
        pr_next += 4;
    }
}

/* returns the smallest p^2 dividing k^2+1, or 0 if k^2+1 is squarefree */
static uint64_t smallest_square(uint64_t k)
{
    uint64_t m = k * k + 1;
    uint64_t r = m;
    uint64_t c = icbrt_u64(m) + 1;
    size_t i;
    ensure_primes(c);
    if ((r & 1) == 0) r >>= 1; /* 4 never divides k^2+1 */
    for (i = 0; i < pr_cnt; i++) {
        uint64_t p = pr[i];
        if (p > c) break;
        if (p * p > r) return 0; /* r is 1 or prime */
        if (r % p == 0) {
            r /= p;
            if (r % p == 0) return p * p;
        }
    }
    /* all prime factors of r exceed cbrt(m): r has at most two of them */
    if (r > 1 && is_square_u64(r)) return r;
    return 0;
}

seq_t a(idx_t n)
{
    while (res_cnt < (size_t)n) {
        uint64_t s = smallest_square(next_k);
        next_k++;
        if (s) {
            if (res_cnt == res_cap) {
                res_cap = res_cap ? res_cap * 2 : 1024;
                res_sq = (uint64_t *)realloc(res_sq, res_cap * sizeof(uint64_t));
            }
            res_sq[res_cnt++] = s;
        }
    }
    return (seq_t)res_sq[n - 1];
}
