/*
 * A276035  Least k such that n divides phi(k^k) (k > 0).
 * https://oeis.org/A276035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that n divides φ(k^k) (k > 0). φ(k^k) =
 *            k^(k‑1) · ∏_{p|k}(p‑1). The function searches k from 1 upward
 *            and stops at the first one that satisfies the divisibility test
 * Range:     u8 n <= 240, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Small‑prime‑factor sieve (spf[i] = smallest prime factor of i, 0 for 1). */
/* The sieve is expanded automatically as needed. */
static idx_t *spf = NULL;
static idx_t  spf_limit = 0;

static void build_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    void *p = realloc(spf, (limit + 1) * sizeof(idx_t));
    if (!p) exit(1);
    spf = (idx_t *)p;
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    spf[0] = 0; spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                      /* i is prime */
            spf[i] = i;
            if (i > limit / i) continue;        /* avoid overflow on i*i */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    spf_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Safe modular multiplication: result = (a * b) mod mod.
   Works for any seq_t that supports +, & and %= operations, including
   built‑in unsigned types and arbitrary‑precision integer classes. */
static inline seq_t mul_mod(seq_t a, seq_t b, idx_t mod)
{
    if (mod == 0) return 0;                      /* avoid % 0 */

    seq_t res = 0;
    seq_t aa  = a % (seq_t)mod;
    idx_t bb  = (idx_t)b;            /* cast for loop condition */

    while (bb) {
        if (bb & 1U) {
            res = (res + aa) % (seq_t)mod;
        }
        aa = (aa + aa) % (seq_t)mod;
        bb >>= 1U;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Modular exponentiation: base^exp mod mod.  exp is idx_t. */
static seq_t pow_mod(seq_t base, idx_t exp, idx_t mod)
{
    if (mod == 0) return 0;
    seq_t result = 1 % (seq_t)mod;
    seq_t b = base % (seq_t)mod;
    idx_t e = exp;
    while (e) {
        if (e & 1U) result = mul_mod(result, b, mod);
        b = mul_mod(b, b, mod);
        e >>= 1U;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Public function: a(n) returns the sequence value for index n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but keep safe */
    if (n == 1) return 1;           /* a(1) = 1 by definition */

    /* Ensure trial division up to n. */
    build_spf(n);

    for (idx_t k = 1; k <= n; ++k) {          /* k > 0 */
        seq_t mod_phi = pow_mod((seq_t)k, k - 1, n);

        /* Multiply by (p-1) for each distinct prime divisor of k. */
        idx_t tmp = k;
        seq_t prod = 1 % (seq_t)n;
        idx_t last_p = 0;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            if (p != last_p) {
                prod = mul_mod(prod, (seq_t)(p - 1), n);
                last_p = p;
            }
            while (tmp % p == 0) tmp /= p;
        }

        seq_t phi_kk = mul_mod(mod_phi, prod, n);
        if (phi_kk == 0) return (seq_t)k;   /* first suitable k found */
    }

    /* The loop must always terminate because k = n works. */
    return (seq_t)n;
}
