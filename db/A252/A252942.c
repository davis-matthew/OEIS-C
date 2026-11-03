/*
 * A252942  Smallest prime of the form "Concatenate(m,n,m)".
 * https://oeis.org/A252942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest prime that equals concat(m, n, m) for some m ≥ 1
 * Range:     u8 n <= 0, u16 n <= 10, u32 n <= 1110, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 8562 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

seq_t a(idx_t n)
{
    for (idx_t m = 1; ; ++m)
    {
        /* skip m whose last digit forces the candidate to be even or divisible by 5 */
        idx_t last = m % 10;
        if ((last & 1) == 0 || last == 5)
            continue;

        /* digit lengths of m and n */
        int dm = num_digits((seq_t)m);
        int dn = num_digits((seq_t)n);

        /* powers of 10 needed for the concatenation */
        seq_t pow_dm    = pow10((idx_t)dm);          /* 10^dm   */
        seq_t pow_dn_dm = pow10((idx_t)(dn + dm));   /* 10^(dn+dm) */

        /* candidate = m * 10^(dn+dm) + n * 10^dm + m */
        seq_t cand = ((seq_t)m) * pow_dn_dm
                   + ((seq_t)n) * pow_dm
                   + ((seq_t)m);

        if (is_prime_big(cand))
            return cand;
    }
}
