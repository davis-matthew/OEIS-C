/*
 * A290967  Smallest known example of a 3 X 3 X 3 generalized arithmetic
 *          progression (GAP) of 27 primes, listed in increasing order.
 * https://oeis.org/A290967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: full, pass, 27 terms through n = 27 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A290967: the 27 primes forming the smallest
   3×3×3 generalized arithmetic progression with differences 2904,
   3150, and 7440, sorted in increasing order.  The function searches
   for the smallest base that yields 27 primes, builds the array,
   sorts it once, and returns the (n‑1)th element.  For n > 27 the
   function returns 0. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
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

static int seq_compare(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a > b) - (a < b);
}

seq_t a(idx_t n)
{
    /* static cache so the search runs only once */
    static bool ready = false;
    static seq_t cube[27];

    /* differences in coordinates (OEIS A153412) */
    const seq_t d1 = (seq_t)2904;
    const seq_t d2 = (seq_t)3150;
    const seq_t d3 = (seq_t)7440;

    if (!ready) {
        seq_t base = 2;
        /* find the smallest base giving 27 primes */
        for (;;) {
            bool allprime = true;
            for (idx_t i = 0; i < 3 && allprime; ++i)
                for (idx_t j = 0; j < 3 && allprime; ++j)
                    for (idx_t k = 0; k < 3; ++k) {
                        seq_t val = base + i*d1 + j*d2 + k*d3;
                        if (!is_prime_big(val)) {
                            allprime = false;
                            break;
                        }
                    }
            if (allprime)
                break;
            ++base;
        }
        /* collect the 27 values */
        idx_t pos = 0;
        for (idx_t i = 0; i < 3; ++i)
            for (idx_t j = 0; j < 3; ++j)
                for (idx_t k = 0; k < 3; ++k)
                    cube[pos++] = base + i*d1 + j*d2 + k*d3;
        /* sort ascending */
        mem_qsort(cube, 27, sizeof(seq_t), seq_compare);
        ready = true;
    }

    if (n < 1 || n > 27)
        return (seq_t)0;
    return cube[n - 1];
}
