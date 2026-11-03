/*
 * A348999  a(n) = A348929(A276086(n)), where A348929(n) = gcd(n, A003959(n)),
 *          A003959 is multiplicative with a(p^e) = (p+1)^e, and A276086 gives
 *          the prime product form of primorial base expansion of n.
 * https://oeis.org/A348999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = gcd(A276086(n), A003959(A276086(n)))
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 598, u64 n <= 11550,
 *            u128 n <= 11550, big any n
 * Validated: standard, pass, 118 terms through n = 9591 (2026-11-01)
 * Helpers:   gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where A003959 is multiplicative with a(p^e) = (p+1)^e,
    and A276086(n) is the product ∏ p_i^d_i with the primorial‑base
    digits d_i of n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

#define MAX_PRIMES 32   /* enough for n ≤ 10000 */

seq_t a(idx_t n)
{
    /* A276086(0) is defined to be 1; gcd(1,1)=1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Build primorials primorial[i] = ∏_{k=1}^{i} p_k, with primorial[0]=1.
       Store them until the next would exceed n. */
    seq_t primorial[MAX_PRIMES];
    primorial[0] = (seq_t)1;
    size_t count = 0;          /* last index stored in primorial[] */
    seq_t cur = (seq_t)1;

    while (count + 1 < MAX_PRIMES) {
        idx_t prime = get_prime((idx_t)(count + 1));   /* get kth prime, k=1⇒2 */
        seq_t next = cur * (seq_t)prime;
        if (next > (seq_t)n) {
            break;
        }
        primorial[++count] = next;
        cur = next;
    }

    /* Highest primorial index that is ≤ n. */
    size_t max_i = count;

    /* Extract primorial‑base digits d_i.  Each digit satisfies
       0 ≤ d_i ≤ p_{i+1}-1, where p_{i+1} = get_prime(i+2). */
    idx_t digits[MAX_PRIMES];
    for (size_t i = 0; i <= max_i; ++i) {
        digits[i] = 0;
    }

    idx_t rem = n;
    for (int i = (int)max_i; i >= 0; --i) {
        seq_t weight = primorial[(size_t)i];
        idx_t digit = rem / (idx_t)weight;
        idx_t max_digit = get_prime((idx_t)(i + 2)) - 1;
        if (digit > max_digit) {
            digit = max_digit;
        }
        digits[(size_t)i] = digit;
        rem -= digit * (idx_t)weight;
        if (i == 0) break;   /* avoid unsigned underflow */
    }

    /* Compute A276086(n) and A003959(A276086(n)). */
    seq_t m  = (seq_t)1;   /* product of p_i^d_i */
    seq_t X  = (seq_t)1;   /* product of (p_i+1)^d_i */

    for (size_t i = 0; i <= max_i; ++i) {
        idx_t exp = digits[i];
        if (exp == 0) continue;

        seq_t prime  = (seq_t)get_prime((idx_t)(i + 1));     /* p_i */
        seq_t pplus  = prime + 1;                           /* p_i + 1 */

        seq_t pow_m = ipow_idx(prime, exp);
        seq_t pow_X = ipow_idx(pplus, exp);

        m *= pow_m;
        X *= pow_X;
    }

    return gcd(m, X);
}
