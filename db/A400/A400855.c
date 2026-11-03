/*
 * A400855  Values taken by A046073.
 * https://oeis.org/A400855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 145 terms through n = 8540 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  Formula:  a(n) is the n-th smallest distinct value of 
 *  A046073(k) – the number of squares in the multiplicative group of units
 *  modulo k.  A046073(k) is computed factor‑wise:
 *
 *  For each odd prime power p^e | k   :   contrib = p^(e-1) * (p-1)/2
 *  For 2^e | k,  e >= 3              :   contrib = 2^(e-3)
 *  For  e <= 2                         :   contrib = 1
 *  A046073(k) = product of the contributions.
 *
 *  The function builds all values for 1 <= k <= LIMIT, keeps only those
 *  that fit into the native seq_t type (to avoid overflow), sorts,
 *  removes duplicates, and answers queries.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* helper: compute A046073(k) when seq_t is arbitrarily large               */
static seq_t
compute_val_large(uint32_t k, uint32_t *spf)
{
    seq_t prod = 1;
    uint32_t t = k;

    while (t > 1) {
        uint32_t p = spf[t];
        uint32_t e = 0;
        while ((t % p) == 0) {
            t /= p;
            ++e;
        }

        seq_t contrib = 1;
        if (p == 2) {
            if (e <= 2) {
                contrib = 1;
            } else {
                for (uint32_t i = 0; i < e - 3; ++i)
                    contrib *= (seq_t)2;
            }
        } else {
            seq_t pow = 1;
            for (uint32_t i = 0; i < e - 1; ++i)
                pow *= (seq_t)p;
            contrib = pow * ((seq_t)(p - 1) / 2);
        }
        prod *= contrib;
    }
    return prod;
}

/* helper: compute A046073(k) when seq_t is small (<= 128 bits)         */
static bool
compute_val_small(uint32_t k, uint32_t *spf, seq_t *out)
{
    unsigned __int128 prod128   = 1;
    unsigned __int128 seq_max   = (unsigned __int128)-1;  /* maximum of seq_t */
    uint32_t t = k;

    while (t > 1) {
        uint32_t p = spf[t];
        uint32_t e = 0;
        while ((t % p) == 0) {
            t /= p;
            ++e;
        }

        unsigned __int128 contrib128 = 1;
        if (p == 2) {
            if (e <= 2) {
                contrib128 = 1;
            } else {
                /* 2^(e-3) */
                if ((unsigned __int128)1 << (e - 3) > seq_max)
                    return false;           /* overflow – skip k */
                contrib128 = (unsigned __int128)1 << (e - 3);
            }
        } else {
            /* p^(e-1) */
            unsigned __int128 pow128 = 1;
            for (uint32_t i = 0; i < e - 1; ++i) {
                if (pow128 > seq_max / p) return false; /* overflow */
                pow128 *= p;
            }
            /* (p-1)/2 */
            unsigned __int128 factor = (unsigned __int128)(p - 1) / 2;
            if (pow128 > seq_max / factor) return false; /* overflow */
            contrib128 = pow128 * factor;
        }

        if (prod128 > seq_max / contrib128) return false; /* overflow */
        prod128 *= contrib128;
    }

    *out = (seq_t)prod128;
    return true;
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* comparison for qsort over seq_t                                         */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* --------------------------------------------------------------------- */
/* the function requested in the problem statement                         */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *uniq_vals = NULL;
    static size_t uniq_cnt = 0;

    if (!initialized) {
        /* we need enough k's to see at least the first 10 000 distinct values.
           1 000 000 is comfortably enough. */
        const uint32_t LIMIT = 1000000u;

        /* ---------- build smallest prime factor table ----------------- */
        uint32_t *spf = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
        if (!spf) return 0;              /* allocation failure – unlikely */

        for (uint32_t i = 2; i <= LIMIT; ++i) {
            if (spf[i] == 0) {           /* i is prime */
                spf[i] = i;
                if ((uint64_t)i * i <= LIMIT) {
                    for (uint32_t j = i * i; j <= LIMIT; j += i) {
                        if (spf[j] == 0) spf[j] = i;
                    }
                }
            }
        }

        /* ---------- compute values for all k ------------------------ */
        seq_t *staging = mem_new(seq_t, (LIMIT + 1));
        if (!staging) { free(spf); return 0; }

        size_t val_cnt = 0;
        const bool small = (sizeof(seq_t) <= sizeof(unsigned __int128));

        for (uint32_t k = 1; k <= LIMIT; ++k) {
            seq_t val;
            bool ok;
            if (small) {
                ok = compute_val_small(k, spf, &val);
            } else {
                val = compute_val_large(k, spf);
                ok = true;                 /* always fits */
            }
            if (ok) {
                staging[val_cnt++] = val;
            }
        }

        free(spf);   /* no longer needed */

        /* ---------- deduplicate ------------------------------------- */
        mem_qsort(staging, val_cnt, sizeof(seq_t), cmp_seq);

        size_t uniq_l = 0;
        for (size_t i = 0; i < val_cnt; ++i) {
            if (i == 0 || staging[i] != staging[i - 1]) {
                staging[uniq_l++] = staging[i];
            }
        }

        uniq_vals = staging;      /* keep this buffer for queries   */
        uniq_cnt  = uniq_l;
        initialized = true;
    }

    if (n == 0 || (idx_t)uniq_cnt < n) return 0;
    return uniq_vals[(size_t)n - 1];
}
