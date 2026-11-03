/*
 * A341715  a(n) = smallest prime of the form n||n+1||n+2||...||n+k, where ||
 *          denotes decimal concatenation, or -1 if no such prime exists.
 * https://oeis.org/A341715
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, more, nice
 * Formula:   a(n) = smallest prime of the form n||n+1||…||n+k, or -1 if no
 *            such prime exists (offset 2)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 8 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/num_digits_idx.h"
#include "../utils/is_prime_u64.h"

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

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

/* Helper: concatenate a (seq_t) with an idx_t b.
   Returns a * 10^len(b) + b. */
static seq_t concat_seq_idx(seq_t a, idx_t b)
{
    /* digits(b) returns number of decimal digits of b. */
    idx_t d = (idx_t)num_digits_idx(b);
    seq_t pow = pow10(d);
    return a * pow + b;
}

seq_t a(idx_t n)
{
    /* a(2) is the first term; values for n < 2 are undefined */
    if (n < 2) {
        /* No defined value below offset; return -1 sentinel. */
        return (seq_t)(-1);
    }

    /* If n itself is prime, return n. */
    if (is_prime(n)) {
        return (seq_t)n;
    }

    /* Start concatenating from n and n+1. */
    seq_t cur = concat_parts(n, n + 1);
    idx_t k = 1;          /* we have included n+1 so far */

    while (1) {
        /* Test whether the current concatenated number is prime. */
        if (is_prime_big(cur)) {
            return cur;
        }

        /* Append next integer. */
        ++k;
        idx_t next = n + k;   /* next integer to append */
        cur = concat_seq_idx(cur, next);
    }

    /* Unreachable, but required to avoid compiler warnings. */
    return (seq_t)(-1);
}
