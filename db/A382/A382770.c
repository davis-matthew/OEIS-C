/*
 * A382770  Number of powerful k < n such that k and n are coprime.
 * https://oeis.org/A382770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   the number of k < n with gcd(k,n)=1 and k is powerful
 * Range:     u8 n <= 1955, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count powerful numbers k < n that are coprime to n */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/gcd.h"
#include "../utils/build_spf.h"

/* ---------- static state for SPF table ---------------------------------- */
static idx_t *spf = NULL;       /* smallest prime factor array (1‑based) */
static idx_t spf_limit = 0;     /* last index in spf array */

/* rebuild SPF table up to `limit` (inclusive) */
static void rebuild_spf(idx_t limit)
{
    if (spf != NULL) {
        free(spf);
        spf = NULL;
    }
    if (limit < 2) {
        spf_limit = 0;
        return;
    }
    build_spf(limit, &spf);      /* provided by utils */
    spf_limit = limit;
}

/* ---------- static state for list of powerful numbers ------------------- */
static idx_t *powers = NULL;    /* array of powerful numbers (idx_t) */
static size_t powers_cnt   = 0; /* current number of entries */
static size_t powers_cap   = 0; /* allocated capacity (entries) */
static idx_t  powers_limit = 0; /* largest k that has been examined */

/* extend the array if needed */
static void ensure_power_capacity(size_t needed)
{
    if (needed <= powers_cap)
        return;
    /* double capacity while keeping at least one element */
    size_t new_cap = powers_cap == 0 ? 1 : powers_cap;
    while (new_cap < needed)
        new_cap <<= 1;
    powers = (idx_t *)realloc(powers, new_cap * sizeof(idx_t));
    powers_cap = new_cap;
}

/* ---------- helper: test whether an integer is powerful ----------------- */
static bool is_powerful(idx_t x)
{
    if (x == 1)               /* 1 is considered powerful */
        return true;

    idx_t tmp = x;
    while (tmp > 1) {
        idx_t p, exp = 0;
        if (tmp <= spf_limit) {
            p = spf[tmp];
        } else {                      /* fallback trial division */
            p = 2;
            while ((idx_t)p * (idx_t)p <= tmp) {
                if (tmp % p == 0)
                    break;
                ++p;
            }
            if ((idx_t)p * (idx_t)p > tmp)
                p = tmp;              /* tmp is prime */
        }
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        if (exp == 1)                /* exponent 1 → not powerful */
            return false;
    }
    return true;
}

/* ---------- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                     /* a(1) = 0 */

    /* Ensure SPF table covers candidates k < n. */
    if (n - 1 > spf_limit)
        rebuild_spf(n - 1);

    /* Extend list of powerful numbers up to n-1. */
    if (n - 1 > powers_limit) {
        const size_t new_needed = powers_cnt + (size_t)(n - 1 - powers_limit);
        ensure_power_capacity(new_needed);
        for (idx_t k = powers_limit + 1; k < n; ++k) {
            if (is_powerful(k))
                powers[powers_cnt++] = k;
        }
        powers_limit = n - 1;
    }

    /* Count powerful k < n that are coprime to n. */
    seq_t count = 0;
    for (size_t i = 0; i < powers_cnt; ++i) {
        /* All stored powers are <= powers_limit which equals n-1 */
        if (gcd((seq_t)powers[i], (seq_t)n) == 1)
            ++count;
    }
    return count;
}
