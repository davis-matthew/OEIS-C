/*
 * A273290  A273288(n)^Omega(n), where Omega = A001222.
 * https://oeis.org/A273290
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A273288(n)^Omega(n) where A273288(n) is the
 *            median-integer derived from the multiset of prime factors of n:
 *            • If the number of factors (with multiplicity) Ω(n) is odd, it
 *            is the middle prime factor. • If Ω(n) is even, it is the floor
 *            of the average of the two middle primes, replaced by the...
 * Range:     u8 n <= 73, u16 n <= 1041, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8775 (2026-11-03)
 * Helpers:   factor_u64, ipow, is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_u64.h"

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

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

/* Retrieve the prime factor at 0‑based position 'pos' in the sorted
   multiset described by arrays 'pr' (primes) and 'ex' (exponents). */
static uint64_t
prime_at_position(const uint64_t *pr, const uint64_t *ex, size_t cnt, size_t pos)
{
    size_t seen = 0;
    for (size_t i = 0; i < cnt; ++i) {
        size_t next = seen + ex[i];
        if (pos < next)
            return pr[i];
        seen = next;
    }
    /* pos is always valid for proper usage */
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Small fixed workspace sufficient for 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    /* Total number of prime factors with multiplicity. */
    size_t omega = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega += (size_t)exps[i];

    /* Determine the base prime A273288(n). */
    uint64_t base;
    if (omega & 1ULL) {        /* odd number of factors */
        size_t mid = omega / 2;
        base = prime_at_position(primes, exps, cnt, mid);
    } else {                   /* even number of factors */
        size_t low_idx  = omega / 2 - 1;
        size_t high_idx = omega / 2;
        uint64_t p_low  = prime_at_position(primes, exps, cnt, low_idx);
        uint64_t p_high = prime_at_position(primes, exps, cnt, high_idx);

        /* Use 128‑bit arithmetic to avoid overflow. */
        unsigned __int128 sum = (unsigned __int128)p_low + p_high;
        uint64_t floor_median = (uint64_t)(sum / 2);

        /* Largest prime ≤ floor_median. */
        if (is_prime_big((seq_t)floor_median))
            base = (uint64_t)floor_median;
        else
            base = (uint64_t)prev_prime((idx_t)floor_median);
    }

    /* Return base^omega.  Use seq_t for arbitrary‑precision cases. */
    return ipow((seq_t)base, (seq_t)omega);
}
