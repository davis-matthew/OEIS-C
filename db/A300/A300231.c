/*
 * A300231  Filter sequence combining A001065(n) and A009194(n), the sum of
 *          proper divisors of n and gcd(n,sigma(n)).
 * https://oeis.org/A300231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of the pair (A001065(n), A009194(n)) in order of
 *            appearance, where A001065(n) = sum of proper divisors of n and
 *            A009194(n) = gcd(n, σ(n)), and the pair is encoded by the Cantor
 *            pairing function: P(x,y) = (x+y)(x+y+1)/2 + y
 * Range:     u8 n <= 47, u16 n <= 1669, u32 n <= 9506, u64 n <= 9506,
 *            u128 n <= 9506, big any n
 * Helpers:   gcd, sum_proper_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation memoizes the sequence and the used
   Cantor values so that repeated calls to a() are O(1). */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/sum_proper_div.h"

/* Cantor pairing for non‑negative integers. */
static seq_t cantor_pair(seq_t a, seq_t b)
{
    seq_t sum = a + b;
    return sum * (sum + 1) / 2 + b;
}

/* Dynamic array helpers: double capacity in powers of two. */
static void ensure_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap ? *cap * 2 : 8);
        while (newcap < needed) newcap *= 2;
        seq_t *newarr = (seq_t*)malloc(newcap * sizeof(seq_t));
        for (size_t i = 0; i < *cap; ++i) newarr[i] = (*arr)[i];
        free(*arr);
        *arr  = newarr;
        *cap  = newcap;
    }
}

/* Static storage for memoisation. */
static seq_t *cants   = NULL;   /* unique Cantor values */
static size_t  clen   = 0;      /* number of unique Cantor values   */
static size_t  cap    = 0;      /* current capacity of cants array  */

static seq_t *vals    = NULL;   /* a(n) values (1‑based, stored 0‑based) */
static size_t  vlen   = 0;      /* number of computed a(n) values  */
static size_t  vcap   = 0;      /* current capacity of vals array  */

/* ------------------------------------------------------------------ */
/* Public function implementing OEIS A300231                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1 */

    if (n > vlen) {
        /* Prepare for new entries. */
        ensure_cap(&vals, &vcap, (size_t)n);

        /* Compute from vlen+1 up to n (inclusive). */
        for (idx_t i = vlen + 1; i <= n; ++i) {
            seq_t proper = sum_proper_div(i);
            seq_t sigma  = proper + (seq_t)i;          /* σ(i)   */
            seq_t g      = gcd((seq_t)i, sigma);      /* A009194(i) */
            seq_t key    = cantor_pair(proper, g);     /* Cantor value */

            /* Search for key among already seen values. */
            size_t j;
            for (j = 0; j < clen; ++j) {
                if (cants[j] == key) break;
            }

            seq_t val;
            if (j == clen) {                        /* first time */
                ensure_cap(&cants, &cap, clen + 1);
                cants[clen] = key;
                val = clen + 1;                      /* next label */
                ++clen;
            } else {
                val = j + 1;                         /* existing label */
            }
            vals[i - 1] = val;                       /* store a(i) */
        }
        vlen = n;
    }

    return vals[n - 1];
}
