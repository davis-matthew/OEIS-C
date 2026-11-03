/*
 * A253851  Mersenne primes (A000668) of the form 2^sigma(n) - 1 for some n.
 * https://oeis.org/A253851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253851: Mersenne primes of the form 2^sigma(n)-1, with sigma(n) prime.
   The function searches successive n to collect the required number of such primes,
   stores them, sorts once, then returns the nth smallest value. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
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

static seq_t *mersenne_list = NULL;   /* array of found Mersenne primes   */
static size_t mersenne_count = 0;      /* number of primes stored          */
static size_t mersenne_cap   = 0;      /* current capacity of the array    */
static idx_t   next_m        = 1;      /* next integer to test for sigma   */
static bool    sorted        = false;  /* true once the list has been sorted */

seq_t a(idx_t n)
{
    /* Undefined position; not used by the OEIS entry. */
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them. */
    while (mersenne_count < (size_t)n) {
        /* Evaluate sigma(next_m). */
        seq_t sigma = sigma_of(next_m);

        /* sigma must be a prime number. */
        if (is_prime_big(sigma)) {
            idx_t p = (idx_t)sigma;

            /* Compute the candidate Mersenne prime 2^p - 1. */
            seq_t mers = pow2(p);
            mers = mers - (seq_t)1;

            /* The candidate must itself be prime. */
            if (is_prime_big(mers)) {
                /* Reject duplicates (should never happen in practice). */
                bool dup = false;
                for (size_t i = 0; i < mersenne_count; ++i) {
                    if (mersenne_list[i] == mers) {
                        dup = true;
                        break;
                    }
                }
                if (!dup) {
                    /* Ensure capacity and store the new prime. */
                    grow_seq(&mersenne_list, &mersenne_cap, mersenne_count + 1);
                    mersenne_list[mersenne_count++] = mers;
                }
            }
        }
        ++next_m;   /* Advance to the next integer. */
    }

    /* Sort the collection once; subsequent calls use the sorted order. */
    if (!sorted) {
        mem_qsort(mersenne_list, mersenne_count, sizeof(seq_t), cmp_seq_t);
        sorted = true;
    }

    /* Return the (n‑th) smallest Mersenne prime of the searched form. */
    return mersenne_list[(size_t)n - 1];
}
