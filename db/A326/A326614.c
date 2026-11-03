/*
 * A326614  Smallest Euler-Jacobi pseudoprime to base n.
 * https://oeis.org/A326614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 87 terms through n = 4679 (2026-11-01)
 * Helpers:   gcd, is_prime_seq, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest Euler–Jacobi pseudoprime to base n:
   a(n) = smallest odd composite m such that
   gcd(n,m)=1 and n^((m‑1)/2) ≡ (n|m) (mod m). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_seq.h"
#include "../utils/powmod.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    uint64_t m = 3;                      /* start with the first odd >1   */
    for (;; m += 2) {                    /* only odd numbers are checked  */
        if (is_prime_seq((seq_t)m))      /* skip primes                    */
            continue;
        seq_t g = gcd((seq_t)n, (seq_t)m);
        if (g != (seq_t)1)               /* require coprimality            */
            continue;
        idx_t exp = (idx_t)((m - 1) / 2);
        seq_t power = powmod((seq_t)n, exp, (seq_t)m);
        int jsym = jacobi_symbol((uint64_t)n, (uint64_t)m);
        seq_t want = (jsym == -1) ? ((seq_t)m - 1) : (seq_t)1;
        if (power == want)              /* pseudoprime condition met      */
            return (seq_t)m;            /* m is the smallest such value   */
    }
}
