/*
 * A264825  Centered 18-gonal (or octadecagonal) primes.
 * https://oeis.org/A264825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form 9*k^2 - 9*k + 1
 * Range:     u8 n <= 3, u16 n <= 32, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A264825 – centered 18‑gonal (octadecagonal) primes */

/* The value is obtained by searching successive k until the nth prime of the form 9*k^2-9*k+1 is found. */

#include <stdbool.h>
#include <stdint.h>
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

/* Return the n-th centered 18-gonal prime.
   a(1) = 19, a(2) = 109, ... */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n = 0, but benign */

    idx_t found = 0;       /* number of primes discovered so far */

    /* iterate k from 2 upward, generating candidates of the form 9*k^2 - 9*k + 1 */
    for (idx_t k = 2;; ++k)
    {
        /* Convert k to the sequence type before performing arithmetic. */
        seq_t kk    = (seq_t)k;
        seq_t value = (seq_t)9 * kk * kk - (seq_t)9 * kk + (seq_t)1;

        if (is_prime_big(value))
        {
            ++found;
            if (found == n)
                return value;
        }
    }
}
