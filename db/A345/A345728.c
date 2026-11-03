/*
 * A345728  Primes with an odd number of digits and 0 as the middle digit.
 * https://oeis.org/A345728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: enumerate odd‑length integers with middle digit 0
   and test primality.  a(n) = the n‑th prime that has (2k+1) digits
   and a zero in the middle position.  The enumeration is in
   increasing order, so a(1)=101, a(2)=103, … */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* static cache for the primes found so far */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* enumeration state */
    static idx_t cur_d       = 3;          /* odd digit length (3,5,7,…) */
    static idx_t cur_k       = 1;          /* cur_d / 2 */
    static idx_t left_start  = 1;          /* left part lower bound */
    static idx_t left_end    = 9;          /* left part upper bound */
    static idx_t cur_left    = 1;          /* current left part */
    static idx_t right_end   = 9;          /* right part upper bound */
    static idx_t cur_right   = 0;          /* current right part */

    /* pre‑computed powers of ten (seq_t): pow10[i] = 10^i */
    static seq_t pow10[20];
    static bool pow10_inited = false;
    if (!pow10_inited) {
        pow10[0] = 1;
        for (int i = 1; i < 20; ++i)
            pow10[i] = pow10[i - 1] * 10;
        pow10_inited = true;
    }

    /* build new primes until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* construct the candidate number */
        seq_t candidate = ((seq_t)cur_left) * pow10[cur_k + 1] + (seq_t)cur_right;

        if (is_prime_big(candidate)) {
            /* grow cache if needed */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {
                    /* allocation failed – abort gracefully */
                    mem_free(cache);
                    cache = NULL;
                    cache_len = cache_cap = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }

        /* advance to the next candidate */
        if (cur_right < right_end) {
            ++cur_right;
        } else {
            cur_right = 0;
            if (cur_left < left_end) {
                ++cur_left;
            } else {
                /* move to the next odd digit length */
                cur_d += 2;
                cur_k = cur_d / 2;
                left_start = (idx_t)pow10[cur_k - 1];
                left_end   = (idx_t)(pow10[cur_k] - 1);
                cur_left   = left_start;
                right_end  = (idx_t)(pow10[cur_k] - 1);
                cur_right  = 0;
            }
        }
    }

    return cache[n - 1];
}
