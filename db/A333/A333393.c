/*
 * A333393  Primes whose binary representation corresponds to the
 *          characteristic function of 1, 2, 3, 5, 7, ... (1 followed by
 *          primes).
 * https://oeis.org/A333393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth prime whose binary representation has 1 exactly at
 *            positions 1 and at all primes ≤ length of the binary string. The
 *            binary string is built iteratively: f(1)=1; for L>1, f(L) =
 *            (f(L-1) << 1) | (is_prime(L) ? 1 : 0)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/mem.h"
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

/* Deterministic Miller–Rabin for 64‑bit values.
   For larger seq_t types, fall back to is_prime_big. */
static bool ss_is_prime_seq(seq_t n)
{
#if defined(__SIZEOF_INT128__)
    /* 64‑bit branch */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        uint64_t x = (uint64_t)n;
        if (x < 2) return false;
        /* quick small‑prime checks */
        const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
        for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
            if (x == small_primes[i]) return true;
            if (x % small_primes[i] == 0) return false;
        }

        /* Miller–Rabin */
        uint64_t d = x - 1;
        int s = 0;
        while ((d & 1ULL) == 0ULL) {
            d >>= 1;
            ++s;
        }
        const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
        for (size_t bi = 0; bi < sizeof(bases)/sizeof(bases[0]); ++bi) {
            uint64_t a = bases[bi];
            if (a % x == 0ULL) continue; /* a == x, skip */
            unsigned __int128 cur = 1;
            unsigned __int128 base = a % x;
            uint64_t exp = d;
            unsigned __int128 mod = x;
            /* fast modular exponentiation */
            while (exp > 0) {
                if (exp & 1ULL) cur = (cur * base) % mod;
                base = (base * base) % mod;
                exp >>= 1;
            }
            if (cur == 1 || cur == (mod - 1)) continue;
            bool composite = true;
            for (int r = 1; r < s; ++r) {
                cur = (cur * cur) % mod;
                if (cur == mod - 1) { composite = false; break; }
            }
            if (composite) return false;
        }
        return true;
    } else {
        /* larger than 64‑bit: rely on provided big‑int test */
        return is_prime_big(n);
    }
#else
    /* No __int128 support – use the big‑int test for all sizes. */
    return is_prime_big(n);
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined for n < 1 */

    /* Static cache for memoization */
    static seq_t *cache      = NULL;   /* prime values */
    static size_t cache_len = 0;       /* number stored */
    static size_t cache_cap = 0;       /* allocated capacity */
    static idx_t  last_k    = 1;       /* last processed length L */
    static seq_t  last_f    = 1;       /* f(last_k) */

    /* Ensure the cache can hold at least n elements. */
    if (cache_len < n) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap;
        while (new_cap < n) new_cap <<= 1;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }

    /* Generate new terms until we have at least n primes. */
    while (cache_len < n) {
        ++last_k;                      /* next length L */
        last_f <<= 1;                  /* shift left */
        if (is_prime(last_k))          /* set bit if L is prime */
            ++last_f;
        if (ss_is_prime_seq(last_f))   /* store if the new value is prime */
            cache[cache_len++] = last_f;
    }

    return cache[n - 1];
}
