/*
 * A360490  a(n) = (1/2) * A241102(n).
 * https://oeis.org/A360490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * A241102(n) (primes obtained as half the
 *            difference * of two cubes of primes). This implementation
 *            generates twin prime * pairs (p, p+2), computes m = p+1, forms
 *            the candidate * 3*m^2 + 1, and keeps it if it is prime. Results
 *            are cached so that * calls in any order are fast
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* cached terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* the next odd number to test as the first of a twin prime pair */
    static uint64_t candidate = 3;

    /* grow cache until the n-th term is available */
    while (cache_len < (size_t)n) {
        /* look for the next twin prime pair (p, p+2) */
        uint64_t p = 0, q = 0;
        while (true) {
            /* ensure candidate stays odd */
            if ((candidate & 1ULL) == 0)
                candidate++;

            /* test candidate and candidate+2 for primality */
            if (!is_prime_big((seq_t)candidate)) {
                candidate += 2;
                continue;
            }
            uint64_t cand_q = candidate + 2;
            if (!is_prime_big((seq_t)cand_q)) {
                candidate += 2;
                continue;
            }

            /* twin prime pair found */
            p = candidate;
            q = cand_q;
            /* next search should start at q for the following pair */
            candidate = cand_q;
            break;
        }

        /* compute the candidate value 3*(p+1)^2 + 1 */
        uint64_t m = p + 1;
        seq_t m_t  = (seq_t)m;
        seq_t val  = (seq_t)3 * m_t * m_t + (seq_t)1;

        /* if the value is prime, store it */
        if (is_prime_big(val)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = alloc_seq_array(new_cap);
                for (size_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                if (cache != NULL)
                    free_seq_array(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    return cache[(size_t)(n - 1)];
}
