/*
 * A380943  Primes written in decimal representation by the concatenation of
 *          primes p and q such that the concatenation of q and p also forms a
 *          prime.
 * https://oeis.org/A380943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th prime p that can be written as the
 *            concatenation * of two primes a and b such that the
 *            concatenation of b and a is also prime. * The split must not
 *            introduce leading zeros in either part
 * Range:     u8 n <= 2, u16 n <= 145, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 959 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "../utils/num_digits0.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* static storage for generated terms */
static seq_t *memo_terms = NULL;        /* array of found terms */
static size_t memo_len  = 0;            /* number of stored terms     */
static size_t memo_cap  = 0;            /* capacity of memo_terms    */

/* last candidate examined, used for incremental generation */
static idx_t last_checked = 1;          /* start at 1; will be moved to 3 on first call */

/* test whether a prime p satisfies the required property */
static bool has_property(idx_t p)
{
    uint64_t d = num_digits0(p);       /* number of decimal digits */
    if (d < 2) return false;           /* need at least two digits */

    uint64_t divisor = 10ULL;          /* 10^k, where k = number of digits of right part */
    for (uint64_t k = 1; k < d; ++k, divisor *= 10ULL)
    {
        idx_t low  = p % divisor;      /* right part */
        idx_t high = p / divisor;      /* left part  */

        /* low must have exactly k digits, i.e. no leading zeros */
        if (low < divisor / 10ULL)     /* divisor/10 == 10^(k-1) */
            continue;

        if (!is_prime_big(low) || !is_prime_big(high))
            continue;

        seq_t joined = concat_parts(low, high);   /* reversed concatenation b||a */
        if (is_prime_big(joined))
            return true;
    }
    return false;
}

/* generate primes and fill memo until it contains at least n terms */
static void generate_until(idx_t n)
{
    if (!memo_terms) {
        memo_terms = NULL; memo_len = 0; memo_cap = 0;
    }

    /* starting point for candidate generation */
    if (last_checked < 3)
        last_checked = 3;
    else if ((last_checked & 1) == 0)
        last_checked++;

    while (memo_len < n) {
        idx_t cand = last_checked;
        for (;;) {
            if (is_prime_big(cand) && has_property(cand)) {
                /* store the term */
                append_candidate(&memo_terms, &memo_len, &memo_cap, cand);
                last_checked = cand + 2;   /* next odd candidate */
                break;                     /* continue outer loop */
            }
            cand += 2;                     /* skip even numbers */
            if (cand == 0)                 /* overflow guard */
                break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for offset 1, but return 0 */

    generate_until(n);                  /* ensure memo has at least n terms */
    return memo_terms[n - 1];
}
