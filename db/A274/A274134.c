/*
 * A274134  Primes p such that both ror(p) and rol(p) are also primes, where
 *          ror(x)=A038572(x) is x rotated one binary place to the right,
 *          rol(x)=A006257(x) is x rotated one binary place to the left.
 * https://oeis.org/A274134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A274134: primes p for which both one‑place binary rotations of p are prime.
   a(n) = the n‑th such prime (offset 1).  The algorithm simply scans
   ascending candidates, tests primality with is_prime_big, computes
   right/left rotations, and counts qualifying primes until the
   requested index is reached.  The code is valid in C11 and C++17
   and works for any seq_t / idx_t provided by the build. */
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

/* Count the number of bits needed to represent x (x > 0).  Works for
   arbitrary‑precision seq_t types because the shift operators are
   defined for seq_t. */
static unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Return the n‑th element of sequence A274134. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                                 /* offset 1 */

    idx_t count = 0;                                      /* found count */
    seq_t candidate = 3;                                  /* first odd >2 */

    while (1) {
        if (is_prime_big(candidate)) {
            unsigned k = bitlen_seq_t(candidate);

            /* right rotation */
            seq_t ror = (candidate >> 1) | ((candidate & 1) << (k - 1));

            /* left rotation */
            seq_t mask = ((seq_t)1 << k) - 1;
            seq_t rol  = ((candidate << 1) | (candidate >> (k - 1))) & mask;

            if (is_prime_big(ror) && is_prime_big(rol)) {
                ++count;
                if (count == n) {
                    return candidate;
                }
            }
        }

        /* next odd candidate */
        candidate += 2;

        /* protect against unsigned wrap‑around on tiny seq_t types */
        if (candidate < 3) break;
    }

    /* Should never reach here for n >= 1.  Return 0 as a fallback. */
    return 0;
}
