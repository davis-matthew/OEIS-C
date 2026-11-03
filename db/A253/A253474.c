/*
 * A253474  Numbers n such that the difference between n^2 and largest prime
 *          less than n^2 is not prime.
 * https://oeis.org/A253474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer m such that m^2 minus the largest prime
 *            less than m^2 is not prime (i.e. the difference equals 1 or a
 *            composite number)
 * Range:     u8 n <= 2, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* External linkage */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* next integer to test (start with 2) */
    static idx_t next_candidate = 2;

    /* zero is outside the sequence (offset 1). Return 0 to indicate error. */
    if (n == 0)
        return 0;

    /* Generate more terms until we have at least n of them */
    while (terms_len < n) {
        seq_t sq = (seq_t)next_candidate * (seq_t)next_candidate;

        /* Find the largest prime < sq */
        seq_t p = sq - (seq_t)1;
        while (!is_prime_big(p))
            p -= 1;

        seq_t diff = sq - p;

        /* If the difference is 1 or composite, store the term */
        if (diff == 1 || !is_prime_big(diff)) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 4;
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = next_candidate;
        }

        ++next_candidate;
    }

    return terms[n - 1];
}
