/*
 * A270836  Numbers k such that sigma(k-1) - phi(k-1) = (3*k-5)/2.
 * https://oeis.org/A270836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem, pow2, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A270836: numbers k such that sigma(k-1) - phi(k-1) = (3*k-5)/2.
   All numbers of the form 2^t + 1 for t ≥ 1 satisfy the equation,
   and the only other sporadic terms below 200 000 are 11 and 231.
   For indices beyond the pre‑computed cache we return the appropriate
   2^t + 1 term. The cache is populated once and reused for all calls. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of cached terms (a(1)…a(cache_len)) */
static size_t cache_cap = 0;
static bool cache_built = false;
static idx_t next_exp = 0;     /* exponent of the first 2^t+1 term NOT in cache */

/* ---------------------------------------------------------------------- */
/* Build the cache of all terms <= LIMIT. All numbers of the form
   2^t+1 satisfy the defining equation; other sporadic terms are found
   by brute force. Afterwards we assume that the remaining terms are
   exactly 2^t+1 for larger exponents. */
static void build_cache(void)
{
    if (cache_built) return;
    cache_built = true;

    const idx_t LIMIT = 200000;

    cache_cap = 128;
    cache = mem_new(seq_t, cache_cap);
    if (!cache) abort();

    for (idx_t k = 3; k <= LIMIT; ++k) {
        /* Powers of two give n = k-1 a power of two. */
        if (((k - 1) & (k - 2)) == 0) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
                if (!cache) abort();
            }
            cache[cache_len++] = k;
            continue;
        }

        /* Test the sporadic terms. */
        seq_t sigma = sigma_of(k - 1);
        seq_t phi   = totient_seq(k - 1);
        seq_t lhs   = sigma - phi;
        seq_t rhs   = ((seq_t)(3 * k) - 5) / 2;
        if (lhs == rhs) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
                if (!cache) abort();
            }
            cache[cache_len++] = k;
        }
    }

    /* Determine the first exponent of 2^t+1 that exceeds LIMIT. */
    uint64_t pow = 1;        /* 2^0 */
    idx_t   e   = 0;
    while (pow + 1 <= (uint64_t)LIMIT) {
        ++e;
        pow <<= 1;
    }
    /* e is the exponent such that 2^e+1 > LIMIT, so the next term uses exponent e. */
    next_exp = e;
}

/* ---------------------------------------------------------------------- */
/* Public function required by the build system.  Returns the nth term
   of A270836 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined for n<1 */

    build_cache();

    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* n is beyond the pre‑computed cache – therefore it must be a term
       of the form 2^t+1, where t >= next_exp.  The (n-cache_len)-th such
       term uses exponent = next_exp + (n-cache_len) - 1.  */
    idx_t offset   = n - cache_len;                /* 1‑based offset in powers-of-two terms */
    idx_t exponent = next_exp + offset - 1;        /* exponent of the 2^exponent+1 term */

    seq_t val = pow2(exponent);   /* 2^exponent as seq_t */
    val += (seq_t)1;              /* +1 to obtain 2^exponent + 1 */
    return val;
}
