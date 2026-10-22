/*
 * A387291  Integers x such that there exist two numbers y,z with x <= y <= z
 *          such that psi(x) = psi(y) = psi(z) = (x + y + z)/2.
 * https://oeis.org/A387291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387291 – integers x such that there exist y,z with
   x <= y <= z and ψ(x)=ψ(y)=ψ(z)=(x+y+z)/2.
   ψ is the Dedekind psi function. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

/* Global state for the Dedekind ψ sieve */
static idx_t current_limit = 0;      /* highest n having ψ known */
static idx_t *spf = NULL;            /* smallest prime factor array   */
static seq_t *psi_arr = NULL;        /* ψ[0..current_limit]           */

/* Grow the ψ table to cover at least `needed`. */
static void extend_limit(idx_t needed)
{
    if (needed <= current_limit)
        return;

    idx_t new_limit = needed * 2;
    if (new_limit < 1024)
        new_limit = 1024;

    free(spf);
    free(psi_arr);

    /* Build smallest prime factor table up to new_limit. */
    build_spf(new_limit, &spf);
    current_limit = new_limit;

    /* Allocate ψ array (C++ cast needed) */
    psi_arr = (seq_t *)malloc(sizeof(seq_t) * (new_limit + 1));
    psi_arr[0] = 0;
    psi_arr[1] = 1;     /* ψ(1) = 1 */

    /* Compute ψ for every n up to new_limit. */
    for (idx_t n = 2; n <= new_limit; ++n) {
        seq_t prod = 1;
        idx_t m = n;
        while (m > 1) {
            idx_t p = spf[m];
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t part = 1;
            for (idx_t i = 0; i < e - 1; ++i)
                part *= (seq_t)p;
            part *= (seq_t)(p + 1);
            prod *= part;
        }
        psi_arr[n] = prod;
    }
}

/* Storage for found sequence values */
static size_t seq_len = 0;
static size_t seq_cap = 0;
static seq_t *seq = NULL;

/* Grow the sequence array to at least `need`. */
static void extend_seq(size_t need)
{
    if (need <= seq_cap)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < need)
        new_cap *= 2;

    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static idx_t tested_x = 0;       /* last candidate examined */

    while (seq_len < (size_t)n) {
        ++tested_x;
        extend_limit(tested_x);

        seq_t psi_x = psi_arr[tested_x];

        /* Compute T = 2*ψ(x) - x.  T represents y+z. */
        seq_t temp = (seq_t)2 * psi_x - (seq_t)tested_x;
        idx_t T = (idx_t)temp;   /* assume fits into idx_t */

        /* Ensure ψ values are available up to T. */
        extend_limit(T);

        idx_t y_max = T / 2;      /* y <= z => y <= T/2 */
        bool found = false;

        for (idx_t y = tested_x; y <= y_max; ++y) {
            if (psi_arr[y] != psi_x)
                continue;
            idx_t z = T - y;
            if (psi_arr[z] == psi_x) {
                extend_seq(seq_len + 1);
                seq[seq_len++] = (seq_t)tested_x;
                found = true;
                break;
            }
        }
        (void)found; /* value used only to avoid unused warning */
    }

    return seq[(size_t)n - 1];
}
