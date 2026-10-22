/*
 * A347228  Dirichlet inverse of A344695, gcd(sigma(n), psi(n)).
 * https://oeis.org/A347228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n > 1, * a(n) = –∑_{d|n, d<n} A344695(n/d) · a(d),
 *            * where A344695(k) = gcd( σ(k), ψ(k) ), * σ(k) = sum of divisors
 *            of k, * ψ(k) = ∏_{p^e||k} p^{e‑1}·(p+1)
 * Range:     i8 n <= 31, i16 n <= 1023, i32 n <= 9713, i64 n <= 9713,
 *            i128 n <= 9713, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- standard headers ---- */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* --- small helpers ---- */

/* Euclidean gcd for signed integer type seq_t */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    if (a < (seq_t)0) a = -a;
    if (b < (seq_t)0) b = -b;
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Trial‑division factorisation (p^e kept in parallel arrays) */
static void factor(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[*cnt] = p;
            exps[*cnt]   = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        primes[*cnt] = n;
        exps[*cnt]   = 1;
        ++(*cnt);
    }
}

/* Sum‑of‑divisors σ(n) */
static seq_t sigma_uint64(uint64_t n)
{
    if (n == 1) return (seq_t)1;
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor(n, primes, exps, &cnt);
    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)1;
        seq_t pow  = (seq_t)1;
        for (uint64_t e = 0; e < exps[i]; ++e) {
            pow *= (seq_t)primes[i];
            term += pow;
        }
        res *= term;
    }
    return res;
}

/* ψ(n) = ∏ p^{e‑1}·(p+1) */
static seq_t psi_uint64(uint64_t n)
{
    if (n == 1) return (seq_t)1;
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor(n, primes, exps, &cnt);
    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pow = (seq_t)1;
        for (uint64_t e = 1; e < exps[i]; ++e) pow *= (seq_t)primes[i];
        seq_t part = pow * ((seq_t)primes[i] + (seq_t)1);
        res *= part;
    }
    return res;
}

/* A344695(k) = gcd( σ(k), ψ(k) ) */
static seq_t A344695_of(uint64_t k)
{
    seq_t s = sigma_uint64(k);
    seq_t p = psi_uint64(k);
    return gcd_seq(s, p);
}

/* Produce all divisors of n (unordered).  Caller supplies enough space. */
static void divisors(uint64_t n, uint64_t *divs, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[(*cnt)++] = d;
            uint64_t other = n / d;
            if (other != d) divs[(*cnt)++] = other;
        }
    }
}

/* --- memoised Dirichlet inverse a(n) ---- */

static seq_t *a_cache = NULL;
static bool *a_done  = NULL;
static idx_t  cache_cap = 0;

/* Ensure storage exists for indices up to n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;
    idx_t new_cap = n * 2;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_done  = (bool  *)realloc(a_done , new_cap * sizeof(bool  ));
    for (idx_t i = cache_cap; i < new_cap; ++i) {
        a_cache[i] = (seq_t)0;
        a_done[i]  = false;
    }
    cache_cap = new_cap;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* safety, not part of OEIS data */

    idx_t idx = n - 1;            /* cache is 0‑based */
    ensure_cache(n);

    if (a_done[idx]) return a_cache[idx];

    if (n == (idx_t)1) {
        a_cache[idx] = (seq_t)1;
        a_done[idx]  = true;
        return a_cache[idx];
    }

    /* compute a(n) using the Dirichlet‑inverse recurrence */
    uint64_t divs[128];
    size_t divcnt;
    divisors((uint64_t)n, divs, &divcnt);

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        if (d == (uint64_t)n) continue;                      /* exclude n itself */
        seq_t a_d = a((idx_t)d);                             /* memoised recursion */
        seq_t f   = A344695_of((uint64_t)n / d);             /* A344695(n/d) */
        sum += f * a_d;
    }

    seq_t result = -sum;
    a_cache[idx] = result;
    a_done[idx]  = true;
    return result;
}
