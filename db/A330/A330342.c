/*
 * A330342  a(n) is the smallest k such that b^(n-1) == b^k (mod n) for all
 *          integers b.
 * https://oeis.org/A330342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k such that b^(n-1) == b^k (mod n) for all
 *            integers b. This implementation computes λ(n) (Carmichael
 *            function), then tests k = (n-1) mod λ(n): - if the property
 *            holds, a(n) = k; - otherwise a(n) = λ(n) + k
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* ----------------------------------------------------------------------- */
/* auxiliary functions                                                     */
/* ----------------------------------------------------------------------- */

static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static idx_t lcm_idx(idx_t a, idx_t b)
{
    return a / gcd_idx(a, b) * b;
}

/* Carmichael function λ(n) for n ≥ 2 */
static idx_t lambda_of(idx_t n)
{
    idx_t lam = 1;
    idx_t tmp = n;

    /* handle factor 2 separately because λ(2^e) varies */
    if (tmp % 2 == 0) {
        int e = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            e++;
        }
        idx_t part;
        if (e == 1)
            part = 1;              /* λ(2) = 1 */
        else if (e == 2)
            part = 2;              /* λ(4) = 2 */
        else
            part = 1ULL << (e - 2);/* λ(2^e) = 2^(e-2), e ≥ 3 */
        lam = lcm_idx(lam, part);
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            /* λ(p^e) = (p-1)*p^(e-1)  (p odd) */
            idx_t p_pow = 1;
            for (int i = 1; i < e; ++i)
                p_pow *= p;
            idx_t part = (p - 1) * p_pow;
            lam = lcm_idx(lam, part);
        }
    }

    /* remaining prime factor > sqrt(tmp) */
    if (tmp > 1) {
        /* exponent 1 */
        idx_t part = tmp - 1; /* λ(tmp) = tmp-1 for prime >2  */
        lam = lcm_idx(lam, part);
    }

    return lam;
}

/* ----------------------------------------------------------------------- */
/* sequence a(n)                                                              */
/* ----------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;    /* a(1) = 0 */

    /* compute λ(n) and the candidate A = (n-1) mod λ(n) */
    idx_t lam = lambda_of(n);
    idx_t A   = (n - 1) % lam;

    /* pre‑compute b^(n-1) (mod n) for all b in [0..n-1] */
    seq_t *vals = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!vals) return (seq_t)0; /* allocation failed, return 0 as safe fallback */

    for (idx_t b = 0; b < n; ++b)
        vals[b] = mod_pow((seq_t)b, (uint64_t)(n - 1), (seq_t)n);

    /* test whether A satisfies the condition */
    bool ok = true;
    for (idx_t b = 0; b < n; ++b) {
        if (mod_pow((seq_t)b, (uint64_t)A, (seq_t)n) != vals[b]) {
            ok = false;
            break;
        }
    }

    free(vals);

    if (ok)
        return (seq_t)A;          /* a(n) = A */
    else
        return (seq_t)(A + lam);  /* a(n) = λ(n) + A */
}
