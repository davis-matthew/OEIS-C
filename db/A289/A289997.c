/*
 * A289997  Numbers n whose trajectory under iteration of the map k ->
 *          (sigma(k)+phi(k))/2 never reaches a fraction (that is, either the
 *          trajectory reaches a prime, which is a fixed point, or diverges to
 *          infinity).
 * https://oeis.org/A289997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-03)
 * Helpers:   is_prime_u64, mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A289997.
 * An integer n belongs to the sequence iff iterating
 *
 *   k  ->  (sigma(k)+phi(k))/2
 *
 * never produces a fraction (i.e., the sum is even at every step)
 * and eventually reaches a prime (which is a fixed point).
 * The function tests consecutive integers starting from 1, keeps
 * those that satisfy the property, caches them, and returns the
 * requested term.  The caching allows calls in arbitrary order.
 */

#include <stdbool.h>
#include <stdlib.h>
/* utilities that implement sigma(n), phi(n) and primality testing */
#include <stdint.h>
#include "../utils/totient_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *a_cache     = NULL;   /* cached sequence values         */
static size_t a_cache_len = 0;      /* number of stored terms          */
static size_t a_cache_cap = 0;      /* allocated capacity              */
static idx_t  next_candidate = 1;    /* next integer to test           */

/* Test whether an integer n belongs to A289997.            */
static bool a289997_good(seq_t n)
{
    if (n == (seq_t)1)
        return true;                /* 1 is a fixed point           */

    /* Iterate the map until a prime is reached or a fraction
       appears (odd sum).  A simple step counter protects
       against accidental infinite loops.                    */
    const int MAX_STEPS = 2000;
    seq_t cur = n;
    for (int step = 0; step < MAX_STEPS; ++step) {
        seq_t sigma = sigma_of((idx_t)cur);
        seq_t phi   = totient_seq(cur);
        seq_t sum   = sigma + phi;

        /* If the sum is odd, the trajectory reaches a fraction –
           n is not in the sequence. */
        if (sum & (seq_t)1)
            return false;

        seq_t nxt = sum / 2;

        /* Reaching a prime means the trajectory eventually
           hits a fixed point. */
        if (is_prime_big(nxt))
            return true;

        /* If the map becomes periodic without reaching a prime,
           we stop.  The only non‑prime fixed point that satisfies
           the condition is 1, handled above. */
        if (nxt == cur)
            return false;

        cur = nxt;
    }

    /* If we hit the step limit, assume the trajectory diverges
       to infinity and therefore qualifies.                    */
    return true;
}

/*---------------------------------------------------------------------*/
/*  Public accessor: required signature seq_t a(idx_t n)               */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (a_cache_len < n) {
        seq_t cand = (seq_t)next_candidate;
        if (a289997_good(cand)) {
            if (a_cache_len + 1 > a_cache_cap) {
                size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
                if (!tmp) abort();   /* memory allocation failed */
                a_cache      = tmp;
                a_cache_cap  = new_cap;
            }
            a_cache[a_cache_len++] = cand;
        }
        ++next_candidate;
    }
    return a_cache[n - 1];
}
