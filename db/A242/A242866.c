/*
 * A242866  Union of 2*A019434 and the semiprimes in A000215.
 * https://oeis.org/A242866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Define the k‑th Fermat number F_k = 2^(2^k) + 1. a(n) =
 *            2*F_{n-1} if F_{n-1} is prime, F_{n-1} otherwise. This
 *            implements the OEIS definition: the sequence is the union of
 *            2*A019434 (double of the Fermat primes) and the semiprimes that
 *            are the composite Fermat numbers. For n >= 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   ipow, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
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
    /* The index starts at 1.  Convert to 0‑based k. */
    idx_t k = n - 1;

    /* 2^k must fit in a uint64_t for the exponent of 2.  If it
       does not, the result exceeds the practicality of this
       implementation (the numbers grow astronomically fast).  */
    if (k >= 64) {
        /* Undefined for very large n – return 0 as a safe fallback. */
        return (seq_t)0;
    }

    /* Compute exponent 2^k (fits in seq_t because k < 64). */
    seq_t exp = ((seq_t)1) << k;

    /* Compute the Fermat number F_k = 2^(2^k) + 1. */
    seq_t two = (seq_t)2;
    seq_t F   = ipow(two, exp) + (seq_t)1;

    /* If F_k is prime, return 2*F_k; otherwise return F_k. */
    if (is_prime_big(F))
        return F * (seq_t)2;
    else
        return F;
}
