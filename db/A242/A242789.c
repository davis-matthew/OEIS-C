/*
 * A242789  Least number k > 1 such that (k^k-n)/(k-n) is an integer.
 * https://oeis.org/A242789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 520, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A242789: least k > 1 such that (k^k - n)/(k - n) is an integer
   (a(n) <= n+1 for all n).  The function searches k from 2 to n+1,
   testing the divisibility condition by computing k^k modulo |k-n|. */

#include <stdint.h>
#include <stdbool.h>

/* modular exponentiation for seq_t using 128‑bit intermediate to avoid overflow */
static seq_t pow_mod_seq(seq_t base, idx_t exp, seq_t mod)
{
    if (mod == 1) return 0;
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    idx_t e = exp;
    while (e) {
        if (e & 1)
            result = (seq_t)(((__uint128_t)result * b) % mod);
        b = (seq_t)(((__uint128_t)b * b) % mod);
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The smallest admissible k is at most n+1, so we start the search
       from k = 2 up to n+1 and stop once a suitable k is found. */
    seq_t res = (seq_t)(n + 1);           /* fallback */
    for (idx_t k = 2; k <= n + 1; ++k) {
        if (k == n) continue;             /* denominator zero would be illegal */
        idx_t diff_i = (k > n) ? (k - n) : (n - k);  /* |k-n| > 0 */
        seq_t diff = (seq_t)diff_i;      /* modulus for the test */
        seq_t rem = pow_mod_seq((seq_t)k, k, diff);
        seq_t n_mod = (seq_t)(n % diff_i);
        if (rem == n_mod) {
            res = (seq_t)k;
            break;
        }
    }
    return res;
}
