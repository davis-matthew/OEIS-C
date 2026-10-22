/*
 * A379947  Positive integers k such that k*(tau(k) - 1)/(sigma(k) - k/d) is
 *          not an integer for all the divisors d of k.
 * https://oeis.org/A379947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 2668, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A379947:
 * a(n) = the n-th positive integer k such that for every divisor d of k,
 *        k*(tau(k)-1) / (sigma(k)-k/d) is NOT an integer.
 *
 * Computation is by brute‑force enumeration of k, testing the condition
 * on each k.  The implementation uses trial division for factorisation
 * and recursive generation of all divisors.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- factorisation ------------------------------------------------- */
static void factorize_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    uint64_t t = 0;

    while ((n & 1ULL) == 0) {      /* factor 2 */
        n >>= 1;
        t++;
    }
    if (t > 0) { pr[c] = 2; ex[c] = t; ++c; }

    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            t = 0;
            while (n % p == 0) { n /= p; ++t; }
            pr[c] = p; ex[c] = t; ++c;
        }
    }
    if (n > 1) { pr[c] = n; ex[c] = 1; ++c; }

    *cnt = c;
}

/* ---------- divisor generation ------------------------------------------- */
static void gen_divs_rec(size_t idx, uint64_t cur, uint64_t *divs, size_t *dp,
                         const uint64_t *pr, const uint64_t *ex, size_t n)
{
    if (idx == n) { divs[(*dp)++] = cur; return; }
    uint64_t p = pr[idx];
    uint64_t val = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divs_rec(idx + 1, cur * val, divs, dp, pr, ex, n);
        val *= p;
    }
}

/* ---------- check whether k qualifies ------------------------------------ */
static bool qualifies(uint64_t k)
{
    if (k == 1) return false;           /* 1 is never in the sequence */

    uint64_t pr[64], ex[64];
    size_t pc;
    factorize_uint64(k, pr, ex, &pc);

    /* compute tau(k) and sigma(k) */
    uint64_t tau = 1;
    seq_t sigma = 1;

    for (size_t i = 0; i < pc; ++i) {
        tau *= (ex[i] + 1);
        /* geometric series for sigma contribution */
        seq_t term = 1;
        seq_t pow = 1;
        for (uint64_t e = 1; e <= ex[i]; ++e) {
            pow *= (seq_t)pr[i];
            term += pow;
        }
        sigma *= term;
    }

    seq_t numerator = (seq_t)k * ((seq_t)tau - 1);

    /* enumerate all divisors of k */
    uint64_t divs[2048];
    size_t nd = 0;
    gen_divs_rec(0, 1, divs, &nd, pr, ex, pc);

    for (size_t i = 0; i < nd; ++i) {
        uint64_t d = divs[i];
        seq_t denom = sigma - (seq_t)(k / d);
        if (denom != 0 && numerator % denom == 0) return false;
    }

    return true;
}

/* ---------- main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static uint64_t next_k   = 1;     /* next k to test */

    /* extend the cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        uint64_t k = next_k++;

        if (qualifies(k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];  /* 1‑based indexing */
}
