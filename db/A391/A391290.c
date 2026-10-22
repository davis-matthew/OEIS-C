/*
 * A391290  Integers x such that there exist two integers 0<y<=z such that
 *          psi(x)^3 = x^3 + y^3 + z^3.
 * https://oeis.org/A391290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 40, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   floor_cube_root, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A391290: integers x such that
   ψ(x)^3 = x^3 + y^3 + z^3 for some integers 0 < y ≤ z.  The
   function a(n) returns the nth term of the sequence.  The index
   starts at 1. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_cube_root.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* ------------------------------------------------------------------ */
/* Cached sequence values.
   seq[] holds the terms found so far, seq_len is the number of terms,
   seq_cap is the allocated capacity.  The table grows as needed. */
static seq_t *seq       = NULL;   /* array of found terms               */
static size_t seq_cap   = 0;      /* allocated capacity                */
static idx_t  seq_len   = 0;      /* number of terms computed          */

/* ------------------------------------------------------------------ */
/* Grow the cached array to hold at least 'need' terms.  The new
   capacity is always at least twice the old one (or 8 if empty). */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;

    seq_t *new_seq = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < seq_len; ++i) new_seq[i] = seq[i];
    if (seq) seq_free_array(seq);
    seq = new_seq;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Compute the Dedekind ψ function of a 64‑bit integer x.  The
   function ψ(x) = x ∏_{p|x} (1 + 1/p) and is computed by
   factorising x into distinct primes.  The result may overflow a
   64‑bit integer, so it is held in seq_t. */
static seq_t psi_of(uint64_t x)
{
    seq_t psi = (seq_t)x;
    uint64_t ux = x;

    for (uint64_t p = 2; p <= ux / p; ++p) {
        if (ux % p == 0) {
            psi = psi * (p + 1) / p;     /* multiply by (p+1)/p            */
            while (ux % p == 0) ux /= p; /* strip all powers of p          */
        }
    }
    if (ux > 1) {
        psi = psi * (ux + 1) / ux;        /* remaining prime factor         */
    }
    return psi;
}

/* ------------------------------------------------------------------ */
/* Test whether a given difference diff can be expressed as a sum
   y^3 + z^3 with 0 < y ≤ z.  The routine enumerates possible y
   and checks whether the remaining part is a perfect cube.  */
static bool has_cube_sum(seq_t diff)
{
    seq_t y = 1;
    while (true) {
        seq_t y3 = y * y * y;
        if (y3 > diff) break;                 /* no further y possible        */
        seq_t remain = diff - y3;
        seq_t z = floor_cube_root(remain);
        seq_t z3 = z * z * z;
        if (z3 == remain && z >= y) return true;
        y = y + 1;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Generate terms until we have at least 'need' entries in the cache.
   The function scans successive integers, finds those that satisfy the
   defining condition, and stores them in the cache. */
static void generate_terms(idx_t need)
{
    /* keep track of the last integer tested so that calls with
       out‑of‑order n are handled correctly. */
    static uint64_t last_x = 0;

    while (seq_len < need) {
        ++last_x;                               /* next integer to test          */
        seq_t x   = (seq_t)last_x;
        seq_t psi = psi_of(last_x);

        seq_t cube_x   = x * x * x;
        seq_t cube_psi = psi * psi * psi;
        if (cube_psi <= cube_x) continue;        /* non‑positive difference       */

        seq_t diff = cube_psi - cube_x;
        if (has_cube_sum(diff)) {
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len++] = x;
        }
    }
}

/* ------------------------------------------------------------------ */
/* External function required by the test harness.  It returns the
   nth term of the sequence.  Requests for n == 0 are undefined and
   return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n==0             */
    if (seq_len < n) generate_terms(n);
    return seq[n - 1];
}
