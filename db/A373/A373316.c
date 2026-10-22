/*
 * A373316  Numbers k such that k and k+2 are both primitive abundant numbers.
 * https://oeis.org/A373316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 95, u32 n <= 208, u64 n <= 208,
 *            u128 n <= 208, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A373316:
 * a(n) = the n-th integer k such that k and k+2 are primitive
 * (abundant) numbers.  A primitive abundant number is abundant
 * and none of its proper divisors is abundant.
 *
 * The function iterates over candidates, testing
 *   1) k is abundant  (σ(k) > 2k)
 *   2) no proper divisor d of k satisfies σ(d) > 2d
 *   3) k+2 also satisfies (1)-(2)
 * Those k are stored in a static cache so that repeated calls
 * remain efficient.
 *
 * This implementation works for any build where seq_t is an integer
 * type (including arbitrary‑precision integer classes) and uses
 * the utilities provided by the build environment (e.g. sigma_of,
 * factor_u64).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

/* Helper: ensure a dynamic array has capacity at least `need` */
static void ensure_cap(size_t need, size_t *cap, void **ptr, size_t elem_size)
{
    if (need > *cap) {
        size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
        if (new_cap < need) new_cap = need;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* Recursive divisor test: set *ok to false if any proper divisor
 * of `k` is abundant. */
static void check_divisors_rec(int idx, uint64_t cur, uint64_t k,
                               uint64_t *pr, uint64_t *exps, size_t cnt,
                               bool *ok)
{
    if (!*ok) return;
    if (idx == (int)cnt) {
        if (cur != k) {
            seq_t sigma = sigma_of(cur);
            if (sigma > ((seq_t)2 * cur)) *ok = false;
        }
        return;
    }
    uint64_t val = 1;
    for (int e = 0; e <= (int)exps[idx]; ++e) {
        check_divisors_rec(idx + 1, cur * val, k, pr, exps, cnt, ok);
        if (!*ok) return;
        val *= pr[idx];
    }
}

/* Test if a 64‑bit integer k is a primitive abundant number. */
static bool is_primitive_abundant(uint64_t k)
{
    /* Step 1: k itself must be abundant. */
    seq_t sigma_k = sigma_of(k);
    if (!(sigma_k > ((seq_t)2 * k))) return false;

    /* Step 2: check each proper divisor. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);

    bool ok = true;
    check_divisors_rec(0, 1, k, primes, exps, cnt, &ok);
    return ok;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;           /* cached terms */
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static uint64_t last_checked = 1;   /* next candidate to test */

    /* Grow cache to hold n terms. */
    while (seq_len < (size_t)n) {
        /* Find next k with both k and k+2 primitive abundant. */
        for (;;) {
            ++last_checked;
            if (is_primitive_abundant(last_checked)) {
                uint64_t candidate = last_checked + 2;
                /* Check upper bound overflow is not a concern here. */
                if (is_primitive_abundant(candidate)) {
                    /* Append k to cache. */
                    ensure_cap(seq_len + 1, &seq_cap, (void **)&seq, sizeof(seq_t));
                    seq[seq_len++] = (seq_t)last_checked;
                    break;
                }
            }
        }
    }

    return seq[n - 1];
}
