/*
 * A247151  Numbers n such that abs(36*n^2 - 810*n + 2753) is prime.
 * https://oeis.org/A247151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n such that abs(36*n^2 - 810*n + 2753) is prime
 * Range:     u8 n <= 1, u16 n <= 53, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
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

/* Static storage for up to 20000 terms – sufficient for n up to 10000 */
static seq_t seq_terms[20000];
static size_t seq_count   = 0;   /* number of terms stored */
static idx_t  next_cand   = 0;   /* next n to test for primality */

/*
 * a(n) returns the nth term of the sequence.
 * The sequence is the list of integers n ≥ 0 for which
 * |36*n^2 - 810*n + 2753| is prime.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, but keep defined behaviour */

    while (seq_count < n)
    {
        /* candidate n to test */
        idx_t cand = next_cand;

        /* compute |36*n^2 - 810*n + 2753| using 64‑bit signed arithmetic */
        int64_t val  = 36LL * (int64_t)cand * (int64_t)cand
                      - 810LL * (int64_t)cand + 2753LL;
        int64_t absv = (val >= 0) ? val : -val;
        seq_t  absnum = (seq_t)absv;

        /* test primality */
        if (is_prime_big(absnum))
        {
            seq_terms[seq_count++] = (seq_t)cand;
        }

        ++next_cand;          /* advance to next integer */
    }

    return seq_terms[n - 1];
}
