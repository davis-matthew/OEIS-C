/*
 * A332075  Odd numbers 2n+1 such that k + 2^m is prime, where k and m are the
 *          odd part and 2-valuation, respectively, of 2n.
 * https://oeis.org/A332075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 6000, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Validated: standard, pass, 77 terms through n = 5667 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A332075
   a(n) = the n‑th odd integer 2k+1 such that (odd part of 2k) + 2^v2(2k) is prime.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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
    if (n < 1) return 0;                    /* undefined for n=0, return 0 */

    idx_t found = 0;                        /* number of sequence terms seen */
    idx_t k     = 1;                        /* current k corresponding to 2k+1 */

    while (found < n) {
        /* current candidate odd number 2k+1 */
        seq_t candidate = (seq_t)2 * (seq_t)k + 1;

        /* compute v2(2k) = 1 + v2(k) */
        unsigned int v2k = __builtin_ctzll((unsigned long long)k);
        unsigned int exp = v2k + 1;          /* exponent of 2 in 2k */

        /* odd part of 2k = k >> v2k */
        seq_t odd_part = (seq_t)(k >> v2k);

        /* compute sum = odd_part + 2^exp */
        seq_t sum = odd_part + ((seq_t)1 << exp);

        if (is_prime_big(sum)) {
            /* this candidate is part of the sequence */
            found++;
            if (found == n) {
                return candidate;
            }
        }
        k++;
    }
    return 0;   /* unreachable */
}
