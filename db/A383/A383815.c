/*
 * A383815  Palindromic primes in A380943.
 * https://oeis.org/A383815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth palindromic prime that can be expressed as a
 * concatenation of two primes p and q such that the concatenation q||p
 * is also prime.  The sequence is enumerated by testing all palindromes
 * in increasing order.
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

/* Return true if val (already known to be prime) can be split as
   described.  len is the number of decimal digits of val. */
static bool has_concat_property(seq_t val, int len)
{
    for (int split = 1; split < len; ++split) {
        seq_t powRight = pow10((idx_t)(len - split));
        seq_t p = val / powRight;        /* left part */
        seq_t q = val % powRight;        /* right part */

        /* reject splits that would introduce leading zeros */
        if (num_digits(p) != split) continue;
        if (num_digits(q) != len - split) continue;

        if (!is_prime_big(p) || !is_prime_big(q)) continue;

        seq_t rev_concat = q * pow10((idx_t)num_digits(p)) + p;
        if (is_prime_big(rev_concat)) return true;
    }
    return false;
}

/* Reverse the decimal digits of x, given that it has d digits. */
static seq_t reverse_digits(seq_t x, int d)
{
    seq_t r = 0;
    for (int i = 0; i < d; ++i) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safe */

    idx_t found = 0;                    /* terms already found */

    for (int len = 3; ; ++len) {        /* minimal palindromic prime length */
        int half = len / 2;

        /* left part must have 'half' digits and the leading digit non‑zero */
        seq_t left_start = pow10((idx_t)(half - 1));
        seq_t left_end   = pow10((idx_t)half) - 1;
        idx_t count_left = (idx_t)(9 * pow10((idx_t)(half - 1)));

        for (idx_t i = 0; i < count_left; ++i) {
            seq_t left = left_start + (seq_t)i;
            seq_t rev_left = reverse_digits(left, half);

            if (len % 2 == 0) {            /* even length: no middle digit */
                seq_t val = left * pow10((idx_t)half) + rev_left;
                if (is_prime_big(val) &&
                    has_concat_property(val, len)) {
                    ++found;
                    if (found == n) return val;
                }
            } else {                       /* odd length: middle digit 0‑9 */
                seq_t left_pow = pow10((idx_t)(half + 1));
                seq_t mid_pow = pow10((idx_t)half);
                for (int mid = 0; mid <= 9; ++mid) {
                    seq_t val = left * left_pow
                              + (seq_t)mid * mid_pow
                              + rev_left;
                    if (is_prime_big(val) &&
                        has_concat_property(val, len)) {
                        ++found;
                        if (found == n) return val;
                    }
                }
            }
        }
    }
}
