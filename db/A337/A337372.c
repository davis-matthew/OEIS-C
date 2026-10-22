/*
 * A337372  Primitively primeshift-abundant numbers: Numbers that are included
 *          in A246282 (k with A003961(k) > 2k), but none of whose proper
 *          divisors are.
 * https://oeis.org/A337372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 621, u32 n <= 9441, u64 n <= 9441,
 *            u128 n <= 9441, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A337372 – Primitively primeshift‑abundant numbers.
    The function a(idx_t n) returns the nth term (n ≥ 1).
    It is computed by checking each integer k serially:
        - compute the shifted‑prime product of k;
        - if that product > 2k, k belongs to A246282;
        - then verify that none of its proper divisors are already known
          members of A246282 (using a simple divisor enumeration);
        - if so, record k as a term of A337372.
    The routine keeps static arrays for the sieve of smallest prime
    factors, a flag array of already‑found terms, and the sequence
    itself, so any call in any order is answered in O(1) after the
    initial buildup.  A moderate sieve limit of 5,000,000 easily
    covers the first 10,000 terms.  Higher limits will be re‑sieved
    automatically if needed.  The implementation uses only integer
    arithmetic and the helper  `next_prime` supplied by the build. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/next_prime.h"

/* --------------------------------------------------------------------- */
/*  Minimal data – no static helper names may be redefined.              */
/* --------------------------------------------------------------------- */
static uint32_t *spf          = NULL;   /* smallest prime factor table */
static bool     *isPrim       = NULL;   /* table of already known terms   */
static seq_t    *seq          = NULL;   /* sequence values                 */
static idx_t    seq_len       = 0;      /* number of terms found so far    */
static idx_t    seq_cap       = 0;      /* capacity of seq                 */
static uint64_t last_k        = 1;      /* last integer examined           */
static size_t   sieve_limit   = 0;      /* current sieve limit             */
static bool     initialized   = false;  /* first‑time initialisation       */

/* --------------------------------------------------------------------- */
/*  Helper: grow the sequence array.                                     */
/* --------------------------------------------------------------------- */
static void seq_grow(void)
{
    if (seq_len >= seq_cap) {
        seq_cap = (seq_cap ? seq_cap * 2 : 16);
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(*seq));
    }
}

/* --------------------------------------------------------------------- */
/*  Helper: sieve smallest prime factor up to `limit`.                   */
/* --------------------------------------------------------------------- */
static void init_sieve(size_t limit)
{
    free(spf);
    free(isPrim);
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    isPrim = (bool *)calloc(limit + 1, sizeof(bool));

    for (size_t i = 0; i <= limit; ++i)
        spf[i] = (uint32_t)i;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {          /* i is prime   */
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
        }
    }
    sieve_limit = limit;
}

/* --------------------------------------------------------------------- */
/*  Helper: extend sieve to a larger limit (doubling).                  */
/* --------------------------------------------------------------------- */
static void extend_sieve(void)
{
    size_t new_limit = sieve_limit * 2;
    init_sieve(new_limit);
}

/* --------------------------------------------------------------------- */
/*  Compute whether k has the shifted‑prime product > 2k.                */
/* --------------------------------------------------------------------- */
static bool shift_abundant(uint64_t k)
{
    seq_t prod   = 1;
    seq_t limit  = (seq_t)k * 2;

    uint64_t tmp = k;
    while (tmp > 1) {
        uint32_t p  = spf[tmp];
        uint32_t e  = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        uint64_t np   = next_prime(p);
        seq_t np_pow = 1;
        for (uint32_t i = 0; i < e; ++i)
            np_pow *= (seq_t)np;

        prod *= np_pow;
        if (prod > limit) return true;       /* early exit */
    }
    return prod > limit;
}

/* --------------------------------------------------------------------- */
/*  Check whether k has a proper divisor that is already a term.        */
/* --------------------------------------------------------------------- */
static bool has_primitive_divisor(uint64_t k)
{
    for (uint64_t d = 1; d * d <= k; ++d) {
        if (k % d == 0) {
            uint64_t d1 = d;
            uint64_t d2 = k / d;
            if (d1 != k && isPrim[d1]) return true;
            if (d2 != k && d2 != d1 && isPrim[d2]) return true;
        }
    }
    return false;
}

/* --------------------------------------------------------------------- */
/*  Main function required by the framework.                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined – return 0 to be safe */

    if (!initialized) {
        init_sieve(5000000);       /* initial sieve limit */
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(*seq));
        initialized = true;
    }

    /* Search until we have at least n terms recorded. */
    while (seq_len < n) {
        ++last_k;                          /* candidate integer */

        /* Ensure bounding arrays are large enough. */
        if (last_k > sieve_limit)
            extend_sieve();

        /* Test the shifted‑prime abundance. */
        if (!shift_abundant(last_k))
            continue;

        /* Reject if any proper divisor is already in the sequence. */
        if (has_primitive_divisor(last_k))
            continue;

        /* Record new term. */
        isPrim[last_k] = true;
        seq_grow();
        seq[seq_len++] = (seq_t)last_k;
    }

    return seq[n - 1];
}
