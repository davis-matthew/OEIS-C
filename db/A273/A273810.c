/*
 * A273810  Primes of the form 2^(2^n) + 597.
 * https://oeis.org/A273810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 2^(2^k)+597, where k starts at
 *            0 and is increased until the n-th prime is found
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes 2^(2^k) using pow2(k) to get 2^k,
   then pow2((idx_t)2^k) to obtain 2^(2^k).  It caches found primes
   so subsequent calls are fast. */
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdbool.h>
#include "../utils/pow2.h"
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

extern seq_t pow2(idx_t exp);            /* computes 2^exp */
bool is_prime_big(seq_t n);              /* primality test for big ints */

seq_t a(idx_t n) {
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static idx_t primes_count = 0;
    static idx_t last_k = (idx_t)-1; /* last k processed */

    /* Grow array helper */
    if (!primes) {
        primes_cap = 8;
        primes = mem_new(seq_t, primes_cap);
    }

    /* Search for primes until we have enough to satisfy n */
    for (idx_t k = last_k + 1; primes_count < n; ++k) {
        /* Compute 2^k */
        seq_t exp = pow2((idx_t)k);

        /* If exp exceeds uint64_t range, we cannot proceed safely */
        if (exp > (seq_t)UINT64_MAX) {
            /* No more computable k in this implementation */
            break;
        }

        /* Compute 2^(2^k) */
        idx_t exp_u64 = (idx_t)exp; /* safe due to the above check */
        seq_t val = pow2(exp_u64);

        /* Add 597 */
        val = val + (seq_t)597;

        /* Check primality */
        if (is_prime_big(val)) {
            if (primes_count == primes_cap) {
                primes_cap *= 2;
                primes = mem_resize(seq_t, primes, primes_cap);
            }
            primes[primes_count++] = val;
            if (primes_count >= n) {
                last_k = k;
                return val;
            }
        }
        last_k = k;
    }

    /* If we exit the loop without enough primes, return 0 */
    return (seq_t)0;
}
