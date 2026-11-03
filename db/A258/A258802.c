/*
 * A258802  Least base b >= 2 such that prime(n) is an absolute prime in base
 *          b with at least 2 distinct digits or 0 if no such base exists.
 * https://oeis.org/A258802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1011, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9929 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A258802:  least base b ≥ 2 such that
   prime(n) is an absolute prime in base b with at least two
   distinct digits, or 0 if no such base exists. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
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

/* Recursively test every distinct permutation of the multiset of digits.
   'base' is the radix, 'vals' and 'cnts' contain the distinct digit values
   and their remaining counts, 'num_vals' the number of distinct digits.
   'pos' is the number of remaining positions to fill, 'acc' is the
   value already accumulated for the prefix.  Returns true if all
   permutations form a prime number; false as soon as a single
   non‑prime permutation is found. */
static bool test_permutations(idx_t base,
                              unsigned int *vals, const size_t *cnts_orig,
                              size_t num_vals, size_t pos, seq_t acc)
{
    /* local copy of counts because we modify them */
    size_t cnts[64];
    for (size_t i = 0; i < num_vals; ++i)
        cnts[i] = cnts_orig[i];

    if (pos == 0)
        return is_prime_big(acc);

    for (size_t i = 0; i < num_vals; ++i)
    {
        if (cnts[i] == 0)
            continue;
        cnts[i]--;
        seq_t new_acc = acc * (seq_t)base + (seq_t)vals[i];
        if (!test_permutations(base, vals, cnts, num_vals, pos - 1, new_acc))
        {
            cnts[i]++;   /* restore and abort */
            return false;
        }
        cnts[i]++;       /* backtrack */
    }
    return true; /* should never reach when pos > 0 */
}

/* Test whether 'p' (prime) is an absolute prime in base 'b'.
   The representation must contain at least two distinct digits
   and no zero digit. */
static bool is_absolute_prime_base(seq_t p, idx_t b)
{
    unsigned int digits[64];
    size_t len = 0;
    seq_t tmp = p;

    /* Extract digits of p in base b (least‑significant first). */
    while (tmp > 0)
    {
        digits[len++] = (unsigned int)(tmp % (seq_t)b);
        tmp /= (seq_t)b;
    }
    if (len < 2)
        return false;                     /* need at least two digits */

    /* Check for zero digit and collect distinct digits. */
    unsigned int distinct[64];
    size_t distinct_cnt = 0;
    for (size_t i = 0; i < len; ++i)
    {
        unsigned int d = digits[i];
        if (d == 0)
            return false;                 /* zero invalidates absolute prime */

        bool seen = false;
        for (size_t j = 0; j < distinct_cnt; ++j)
            if (distinct[j] == d) { seen = true; break; }

        if (!seen)
            distinct[distinct_cnt++] = d;
    }
    if (distinct_cnt < 2)
        return false;                     /* need at least two distinct digits */

    /* Build arrays of unique digits and their multiplicities. */
    unsigned int vals[64];
    size_t cnts[64];
    size_t vcnt = 0;
    for (size_t i = 0; i < len; ++i)
    {
        unsigned int d = digits[i];
        bool found = false;
        for (size_t j = 0; j < vcnt; ++j)
        {
            if (vals[j] == d) { cnts[j]++; found = true; break; }
        }
        if (!found)
        {
            vals[vcnt] = d;
            cnts[vcnt] = 1;
            vcnt++;
        }
    }

    /* Recursively test all distinct permutations. */
    return test_permutations(b, vals, cnts, vcnt, len, 0);
}

/* OEIS sequence A258802.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n <= 2)         /* the first two terms are known to be 0 */
        return 0;

    /* obtain the n‑th prime (returns unsigned int, cast to seq_t). */
    seq_t p = (seq_t)get_prime(n);

    /* search for the smallest base b ≥ 2. */
    for (idx_t b = 2; b < (idx_t)p; ++b)
        if (is_absolute_prime_base(p, b))
            return (seq_t)b;

    return 0;           /* none found */
}
