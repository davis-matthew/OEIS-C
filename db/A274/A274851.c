/*
 * A274851  Smallest prime q larger than p=prime(n) such that (p+1)(q+1) is a
 *          square m^2; a(n)=0 if there is no such q.
 * https://oeis.org/A274851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274851:
   a(n) = the smallest prime q > prime(n) such that (prime(n)+1)(q+1) is a perfect square.
   If no such q exists, a(n)=0.  The computation is performed by explicit
   factorisation of prime(n)+1 and a simple search for the first suitable q. */

/* Dependencies from the shared utils */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u64.h"

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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* 1. Retrieve the nth prime p */
    seq_t p_seq = (seq_t)get_prime(n);
    if (p_seq < 2) return 0;      /* safety guard */

    /* 2. Compute p+1 as a 64‑bit integer for factorisation */
    uint64_t p  = (uint64_t)p_seq;
    uint64_t P  = p + 1;           /* p+1 */

    /* 3. Factor P to obtain its square‑free part d */
    uint64_t temp = P;
    uint64_t d    = 1;             /* square‑free part */
    for (idx_t k = 1; /*no init*/ ; ++k) {
        uint32_t prime = get_prime(k);
        uint64_t pr   = (uint64_t)prime;
        if (pr * pr > temp) break; /* remaining part is prime if >1 */
        int cnt = 0;
        while (temp % pr == 0) {
            temp /= pr;
            ++cnt;
        }
        if (cnt & 1)                /* odd exponent → include in d */
            d *= pr;
    }
    if (temp > 1)                   /* leftover prime factor */
        d *= temp;

    /* 4. If d == 1, no suitable q exists (q+1 would have to be a square) */
    if (d == 1) return 0;

    /* 5. Compute a = sqrt((p+1)/d).  Because (p+1)/d is a perfect square. */
    seq_t a_seq = floor_sqrt((seq_t)(P / d));
    seq_t t = a_seq + 1;            /* start search with the next integer */

    seq_t d_seq = (seq_t)d;         /* promote to seq_t for later arithmetic */

    for (;;) {
        /* q = d * t^2 - 1 */
        seq_t tt   = t * t;
        seq_t prod = d_seq * tt;
        seq_t q    = prod - 1;

        if (q <= p_seq) {          /* should not happen once t>a, but guard */
            ++t;
            continue;
        }

        if (is_prime_big(q))
            return q;              /* found the required prime */

        ++t;                       /* try the next integer */
    }
}
