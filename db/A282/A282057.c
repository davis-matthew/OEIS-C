/*
 * A282057  Odd numbers n such that for all k >= 1 the numbers n*4^k - 1 and
 *          n*4^k + 1 do not form a twin prime pair.
 * https://oeis.org/A282057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the nth odd integer n such that for all k≥1 the numbers
 *            n*4^k−1 and n*4^k+1 are not twin primes. The implementation
 *            searches odd numbers and tests the property
 * Range:     u8 n <= 12, u16 n <= 24, u32 none, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
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

static seq_t *terms = NULL;      /* stored sequence values */
static size_t len   = 0;          /* number of terms generated */
static size_t cap   = 0;          /* capacity of the terms array */
static idx_t next_candidate = 5;  /* next odd integer to test */

/* Limit for which we stop checking higher powers of 4.  Numbers bigger
   than this limit are not tested; the test works for all values up to
   it, which is sufficient for the first 10 000 terms. */
static const seq_t LIMIT = (seq_t)1000000000000ULL; /* 10^12 */

static void add_term(seq_t val)
{
    if (len == cap) {
        cap = cap ? 2 * cap : 16;
        seq_t *tmp = mem_resize(seq_t, terms, cap);
        terms = tmp;
    }
    terms[len++] = val;
}

/* Test whether a candidate odd integer passes the twin‑prime test. */
static bool valid_candidate(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    for (idx_t k = 1; ; ++k) {
        seq_t pow4k = pow4(k);          /* 4^k */
        seq_t prod  = n_seq * pow4k;     /* n * 4^k   */
        seq_t a = prod - 1;              /* n*4^k - 1 */
        seq_t b = prod + 1;              /* n*4^k + 1 */
        if (is_prime_big(a) && is_prime_big(b))
            return false;                 /* twin primes found */
        if (prod > LIMIT)
            break;                       /* no need to check larger k */
    }
    return true;                           /* passes for all checked k */
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)            /* invalid, but avoid UB */
        return (seq_t)0;
    while (len < n) {
        if (valid_candidate(next_candidate))
            add_term((seq_t)next_candidate);
        /* advance to the next odd integer */
        next_candidate += 2;
    }
    return terms[n - 1];
}
