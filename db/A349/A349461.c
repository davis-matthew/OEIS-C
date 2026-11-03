/*
 * A349461  Primes of the form m^2 + 9*m + 81.
 * https://oeis.org/A349461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A349461 : primes of the form m^2 + 9*m + 81, in increasing order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
    if (n == 0) {
        /* The sequence offset is 1; we can safely return 0 for n==0. */
        return (seq_t)0;
    }

    /* Static state: prime table, its size, capacity and last processed k. */
    static seq_t *primes = NULL;
    static size_t primes_len = 0, primes_cap = 0;
    static idx_t last_k = 0;

    /* Ensure there is enough space to hold (primes_len+1) elements. */
    if (last_k == 0 && primes_cap == 0) {
        primes_cap = 64;
        primes = mem_resize(seq_t, primes, primes_cap);
    }

    while (primes_len < n) {
        /* For a given k we have two m values:
           m_left  = -4 - k   (negative side)
           m_right = -5 + k   (mirror side, may be negative or positive)  */
        idx_t k = last_k;
        int64_t m_left  = -4 - (int64_t)k;
        int64_t m_right = -5 + (int64_t)k;

        /* Compute p = m^2 + 9*m + 81 using 64‑bit arithmetic.  */
        int64_t v_left  = m_left  * m_left  + 9 * m_left  + 81;
        int64_t v_right = m_right * m_right + 9 * m_right + 81;

        /* Test left value */
        seq_t p_left = (seq_t)v_left;
        if (is_prime_big(p_left)) {
            if (primes_len == 0 || p_left != primes[primes_len - 1]) {
                if (primes_len == primes_cap) {
                    primes_cap <<= 1;
                    primes = mem_resize(seq_t, primes, primes_cap);
                }
                primes[primes_len++] = p_left;
                if (primes_len >= n) break;
            }
        }

        /* Test right value (may be equal to left). */
        seq_t p_right = (seq_t)v_right;
        if (is_prime_big(p_right)) {
            if (primes_len == 0 || p_right != primes[primes_len - 1]) {
                if (primes_len == primes_cap) {
                    primes_cap <<= 1;
                    primes = mem_resize(seq_t, primes, primes_cap);
                }
                primes[primes_len++] = p_right;
                if (primes_len >= n) break;
            }
        }

        ++last_k;
    }

    return primes[n - 1];
}
