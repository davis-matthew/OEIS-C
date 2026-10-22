/*
 * A349177  Odd numbers k for which gcd(k, A003961(k)) = gcd(sigma(k),
 *          A003961(k)) = 1, where A003961(n) is fully multiplicative with
 *          a(prime(k)) = prime(k+1), and sigma is the sum of divisors
 *          function.
 * https://oeis.org/A349177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   factor_u64, mul_mod64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A349177: odd numbers k such that gcd(k, A003961(k)) =
   gcd(σ(k), A003961(k)) = 1.  The function searches for odd
   numbers satisfying the conditions and caches the results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"      /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/next_prime.h"     /* uint64_t next_prime(uint64_t start); */
#include "../utils/mul_mod64.h"      /* uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod); */

static seq_t * _cache       = NULL;  /* cache of found terms */
static size_t _cache_len     = 0;     /* number of terms stored */
static size_t _cache_cap     = 0;     /* cache capacity */
static idx_t  _next_candidate = 1;    /* next odd number to test */

/* Resize the cache if needed. */
static void _ensure_cache_cap(size_t need)
{
    if (_cache_cap >= need) return;
    size_t new_cap = _cache_cap ? _cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* out of memory */
    _cache = tmp;
    _cache_cap = new_cap;
}

/* Return true if odd number cand satisfies both gcd conditions. */
static bool _is_valid_odd(idx_t cand)
{
    /* 1. Factor cand. */
    uint64_t pr[64], ex[64];
    size_t factor_cnt;
    factor_u64((uint64_t)cand, pr, ex, &factor_cnt);

    /* 2. Condition gcd(k, A003961(k)) = 1  */
    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t q = next_prime(pr[i]);          /* next prime after p */
        for (size_t j = 0; j < factor_cnt; ++j) {
            if (pr[j] == q)                     /* p and nextprime(p) both divide k */
                return false;
        }
    }

    /* 3. Condition gcd(σ(k), A003961(k)) = 1  */
    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t q = next_prime(pr[i]);          /* prime that appears in A003961(k) */

        uint64_t prod = 1 % q;                  /* product of σ(p^e) modulo q */

        for (size_t j = 0; j < factor_cnt; ++j) {
            uint64_t p = pr[j];
            uint64_t e = ex[j];
            uint64_t sum = 1 % q;               /* σ(p^e) mod q */
            uint64_t term = 1 % q;              /* p^i mod q, i starts at 0 */

            for (uint64_t t = 1; t <= e; ++t) {
                term = mul_mod64(term, p, q);   /* term = p^t mod q */
                sum += term;
                if (sum >= q) sum -= q;
            }
            prod = mul_mod64(prod, sum, q);
            if (prod == 0) break;               /* early exit */
        }
        if (prod == 0) return false;            /* q divides σ(k) */
    }

    return true;                                 /* all conditions satisfied */
}

/* Return the nth term (offset 1). */
seq_t a(idx_t n)
{
    while (_cache_len < n) {
        idx_t cand = _next_candidate;
        if (_is_valid_odd(cand)) {
            _ensure_cache_cap(_cache_len + 1);
            _cache[_cache_len++] = (seq_t)cand;
        }
        _next_candidate += 2;   /* next odd candidate */
    }
    return _cache[n - 1];
}
