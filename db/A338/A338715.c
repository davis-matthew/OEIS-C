/*
 * A338715  Smallest prime ending with decimal expansion of n, for n
 *          relatively prime to 10.
 * https://oeis.org/A338715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = smallest prime p such that p ≡ m (mod 10^d), where m is
 *            the n‑th positive integer coprime to 10 (i.e. with gcd(m,10)=1)
 *            and d is the number of decimal digits of m
 * Range:     u8 n <= 8, u16 n <= 411, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9177 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for the prime by
   iterating over numbers that end with the digit string
   of m. */

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

seq_t a(idx_t n)
{
    /* Determine the n‑th integer coprime to 10.
       Coprime numbers come in the pattern 1,3,7,9 modulo 10. */
    static const idx_t residues[4] = {1,3,7,9};
    idx_t k = (n - 1) / 4;                   /* block index */
    idx_t r = residues[(n - 1) % 4];          /* residue in {1,3,7,9} */
    idx_t m = 10 * k + r;                    /* the target integer */

    /* Compute 10^digits(m).  This is the step size to preserve the suffix. */
    idx_t tmp = m;
    seq_t pow10 = 1;
    /* At least one digit; loop until tmp becomes zero. */
    do {
        pow10 *= 10;
        tmp /= 10;
    } while (tmp > 0);

    /* Search for the smallest prime ending with the decimal expansion of m. */
    seq_t candidate = (seq_t)m;
    while (!is_prime_big(candidate)) {
        candidate += pow10;
    }
    return candidate;
}
