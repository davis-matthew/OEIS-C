/*
 * A275824  Number of ways to write n as pen(x) + pen(y) + pen (z) + 2*pen(w),
 *          where x,y,z,w are nonnegative integers with x <= y <= z, and
 *          pen(k) denotes the pentagonal number k*(3*k-1)/2.
 * https://oeis.org/A275824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of tuples (x,y,z,w) with 0≤x≤y≤z such that
 *            pen(x)+pen(y)+pen(z)+2*pen(w)=n where pen(k)=k*(3*k-1)/2
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pent_of_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pent_of_k.h"

/* ------------------------------------------------------------------ */
/*  Pentagonal numbers cache                                          */
/* ------------------------------------------------------------------ */
#define PENT_MAX 5000            /* enough for n up to ~10 000 with all builds */

static seq_t pent[PENT_MAX];
static idx_t pent_len = 0;

/* Extend the pent cache until pen(k)>max_val or we reach PENT_MAX. */
static void ensure_pent(idx_t max_val)
{
    if (pent_len == 0) {
        pent[0] = pent_of_k(0);
        pent_len = 1;
    }
    while (1) {
        idx_t k = pent_len;
        seq_t v = pent_of_k(k);

        /* If pen(k) exceeds max_val we are done. */
        if ((idx_t)v > max_val)
            break;

        /* Stop before hitting the static array limit. */
        if (pent_len >= PENT_MAX)
            break;

        pent[pent_len++] = v;
    }
}

/* Binary search for val in the current pent array.  Return index
   or -1 if not found. */
static int find_pent_index(seq_t val)
{
    idx_t lo = 0, hi = pent_len;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        seq_t p = pent[mid];
        if (p == val)
            return (int)mid;
        if (p < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return -1;
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* make sure we have pent numbers up to at least n (the largest
       needed is pen(k)≤n, since pen(k) grows roughly like k²). */
    ensure_pent(n);

    seq_t count = 0;

    /* Outer loop over w: 2*pen(w) ≤ n */
    for (idx_t w = 0; w < pent_len; ++w) {
        idx_t two_pw = (idx_t)2 * (idx_t)pent[w];
        if (two_pw > n)
            break;
        idx_t rem = n - two_pw;

        /* Loop over x: pen(x) ≤ rem */
        for (idx_t x = 0; x < pent_len; ++x) {
            idx_t px = (idx_t)pent[x];
            if (px > rem)
                break;
            idx_t rem2 = rem - px;

            /* Loop over y (≥x): pen(y) ≤ rem2 */
            for (idx_t y = x; y < pent_len; ++y) {
                idx_t py = (idx_t)pent[y];
                if (py > rem2)
                    break;
                idx_t rem3 = rem2 - py;

                /* z must satisfy pen(z)=rem3 and z≥y */
                if (rem3 > (idx_t)pent[pent_len - 1])
                    continue;                 /* no such pentagonal number */
                int z_idx = find_pent_index((seq_t)rem3);
                if (z_idx >= 0 && (idx_t)z_idx >= y)
                    ++count;
            }
        }
    }

    return count;
}
