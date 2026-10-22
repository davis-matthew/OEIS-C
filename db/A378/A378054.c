/*
 * A378054  Numbers k that divide A378053(k) = gcd(Product_{d|k} (d + 1),
 *          Product_{d|k, d>1} (d - 1)).
 * https://oeis.org/A378054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 785, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, report_divisor, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A378054 – Numbers k that divide A378053(k)
 *  A378053(k) = gcd( ∏_{d|k}  (d+1) , ∏_{d|k, d>1} (d-1) )
 *
 *  a(n) returns the nth smallest k (1‑based) with k | A378053(k).
 *  The implementation searches k in increasing order, testing each k
 *  for the divisibility condition using prime‑exponent sums of the two
 *  divisor products.  It caches already found terms for fast repeated
 *  access.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/factor_u64.h"
#include "../utils/seq_realloc.h"
#include "../utils/report_divisor.h"

static struct {
    seq_t *data;   /* array of already found terms             */
    size_t cap;    /* allocated capacity (in elements)         */
    size_t len;    /* number of terms stored (strictly < cap) */
} seq_cache = {NULL, 0, 0};

static uint64_t next_candidate = 1;

/*  Checks whether k satisfies k | A378053(k).  Returns true if
 *  the condition holds for every prime factor p^e of k.          */
static bool good_k(uint64_t k)
{
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);      /* cnt <= 16 for k<2^64 */

    uint64_t sum_f[16] = {0};
    uint64_t sum_g[16] = {0};

    report_divisor(primes, exps, cnt, 0, 1, sum_f, sum_g);

    for (size_t i = 0; i < cnt; ++i) {
        if (sum_f[i] < exps[i] || sum_g[i] < exps[i]) {
            return false;
        }
    }
    return true;
}

/*  Grow the cached array of terms to accommodate at least one more
 *  element, using the seq_realloc utility.                            */
static void store_term(seq_t v)
{
    if (seq_cache.len + 1 > seq_cache.cap) {
        size_t new_cap = seq_cache.cap ? seq_cache.cap * 2 : 8;
        seq_t *new_ptr = seq_realloc(seq_cache.data, seq_cache.cap, new_cap);
        seq_cache.data = new_ptr;
        seq_cache.cap = new_cap;
    }
    seq_cache.data[seq_cache.len++] = v;
}

/*  Public sequence accessor.  Calls will come in any order, but
 *  the internal cache guarantees that each term is computed only
 *  once.                                                                      */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1 – a(0) is undefined */

    while (seq_cache.len < n) {
        if (good_k(next_candidate)) {
            store_term((seq_t)next_candidate);
        }
        ++next_candidate;
    }
    return seq_cache.data[n - 1];
}
