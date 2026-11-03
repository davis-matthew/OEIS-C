/*
 * A392156  a(n) is the number of distinct primes obtainable by inserting
 *          exactly one decimal digit into the decimal representation of
 *          prime(n) (at any position, including before the first digit or
 *          after the last), with the resulting number required to have no
 *          leading zero.
 * https://oeis.org/A392156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1907, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9972 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements A392156:  a(n) = number of distinct primes obtained by
   inserting one decimal digit into the nth prime (prime(n)), with no
   leading zero in the resulting number. */
#include <stdbool.h>
#include <stdlib.h>

/* utilities required for the implementation */
#include <stdint.h>
#include <stddef.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"
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

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* offset is 1; behaviour below 1 irrelevant */

    /* ---------- 1. obtain the nth prime --------------------------------- */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;          /* promote to seq_t */

    /* ---------- 2. number of decimal digits of p ----------------------- */
    int L_int = num_digits(p);
    if (L_int < 0) return 0;          /* safety: should not happen */
    uint32_t L = (uint32_t)L_int;     /* convert to unsigned for loops */

    /* ---------- 3. pre‑compute powers of 10: 10^0 … 10^L ---------------- */
    seq_t pow10[21];                  /* L <= 6 for n <= 10000; 21 is safe */
    pow10[0] = (seq_t)1;
    for (uint32_t i = 1; i <= L; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10;

    /* ---------- 4. generate all candidate primes ------------------------- */
    /* maximum candidates = (L+1) * 10; safe upper bound: 70 for L=6        */
    seq_t cand[100];
    size_t cand_cnt = 0;

    for (uint32_t pos = 0; pos <= L; ++pos) {
        /* pow10_exp = 10^(L-pos) */
        uint32_t exp = L - pos;
        seq_t pow = pow10[exp];

        /* left  = floor(p / pow), right = p % pow          */
        seq_t left  = p / pow;
        seq_t right = p % pow;

        for (uint8_t d = 0; d <= 9; ++d) {
            if (pos == 0 && d == 0) continue;    /* no leading zero */

            /* newnum = (left*10 + d)*pow + right */
            seq_t newnum = ((left * (seq_t)10) + (seq_t)d) * pow + right;

            if (is_prime_big(newnum)) {
                cand[cand_cnt++] = newnum;
            }
        }
    }

    if (cand_cnt == 0) return 0;

    /* ---------- 5. remove duplicates ------------------------------------- */
    mem_qsort(cand, cand_cnt, sizeof(seq_t), cmp_seq_t);

    size_t unique = 1;
    for (size_t i = 1; i < cand_cnt; ++i)
        if (cand[i] != cand[i - 1])
            ++unique;

    return (seq_t)unique;
}
