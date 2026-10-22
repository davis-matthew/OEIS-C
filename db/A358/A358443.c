/*
 * A358443  a(1) = 1. After each newly determined a(n-1), cross out every n-th
 *          number in the line of the positive integers. a(n) will be the
 *          smallest unused number that has not been crossed out.
 * https://oeis.org/A358443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements A358443 via the described sieve.
 * a(1)=1.  For n>1, after a(n‑1) we cross out every n‑th
 * remaining positive integer; a(n) is the smallest remaining number.
 * The sequence terminates after 11 terms; for n>11 this function
 * returns 0.  The algorithm keeps a memoised table of computed
 * terms and a bitmap of numbers that have become terms.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Helper: gcd for unsigned long long                                  */
static inline unsigned long long gcd_ull(unsigned long long a,
                                         unsigned long long b)
{
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Storage for the sequence values and a bitmap of used numbers      */
static seq_t *aseq      = NULL;   /* computed a(n) values    */
static size_t a_cap    = 0;       /* array capacity          */
static size_t a_len    = 0;       /* number of terms stored  */

static unsigned char *used   = NULL;   /* 0 = unused, 1 = a term */
static size_t         used_cap = 0;      /* used array capacity    */

static bool terminated = false;          /* no further terms exist */

/* ------------------------------------------------------------------ */
static void aensure_seq_cap(size_t need)
{
    if (need <= a_cap) return;
    size_t new_cap = (a_cap == 0) ? 16 : a_cap;
    while (new_cap < need) new_cap <<= 1;
    seq_t *newptr = (seq_t *)realloc(aseq, new_cap * sizeof(seq_t));
    aseq = newptr;
    a_cap = new_cap;
}

/* ------------------------------------------------------------------ */
static void used_ensure_cap(size_t need)
{
    if (need <= used_cap) return;
    size_t new_cap = (used_cap == 0) ? 256 : used_cap;
    while (new_cap < need) new_cap <<= 1;
    unsigned char *newptr = (unsigned char *)realloc(used, new_cap);
    memset((void *)(newptr + used_cap), 0, new_cap - used_cap);
    used = newptr;
    used_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Compute terms up to (and including) index n                         */
static void ensure_computed(idx_t n)
{
    if (n == 0) return;
    if (terminated) return;
    while (a_len < (size_t)n && !terminated) {
        /* step = a_len + 1 is the index to compute */
        size_t step = a_len + 1;

        /* residues a[j-1] % j for 2 ≤ j ≤ step */
        idx_t *resid = (idx_t *)malloc((step - 1) * sizeof(idx_t));
        for (idx_t j = 2; j <= (idx_t)step; ++j) {
            /* use seq_t % (seq_t)j conversion and cast to idx_t */
            resid[j - 2] = (idx_t)(aseq[j - 2] % (seq_t)j);
        }

        /* compute lcm of numbers 2…step (fits in 64‑bit for step ≤ 11) */
        unsigned long long lcm = 1ULL;
        for (idx_t j = 2; j <= (idx_t)step; ++j) {
            unsigned long long g = gcd_ull(lcm, (unsigned long long)j);
            lcm = (lcm / g) * j;
            /* lcm ≤ 27720 for step ≤ 11, safe */
        }

        /* ensure bitmap large enough */
        used_ensure_cap((size_t)lcm + 1);

        /* search for the smallest candidate in [1, lcm]      */
        idx_t candidate = 0;
        for (idx_t k = 1; k <= (idx_t)lcm; ++k) {
            if (used[(size_t)k]) continue;             /* already a term   */
            bool ok = true;
            for (idx_t j = 2; j <= (idx_t)step; ++j) {
                if ((k % j) == resid[j - 2]) {          /* would be crossed */
                    ok = false;
                    break;
                }
            }
            if (ok) {
                candidate = k;
                break;
            }
        }

        free(resid);

        if (candidate == 0) {   /* no such number → sequence ends */
            terminated = true;
            return;
        }

        /* store the new term */
        aensure_seq_cap(a_len + 1);
        aseq[a_len] = (seq_t)candidate;
        used[(size_t)candidate] = 1;
        a_len++;
    }
}

/* ------------------------------------------------------------------ */
/* Public function defined by the problem statement:                 */
seq_t a(idx_t n)
{
    if (n == 0)            return 0;   /* undefined outside offset */
    if (terminated)        return 0;   /* sequence ended */
    ensure_computed(n);

    if ((idx_t)a_len >= n)      /* term already computed */
        return aseq[(size_t)n - 1];

    return 0;   /* beyond the finite length (n > 11) */
}
