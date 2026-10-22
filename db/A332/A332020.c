/*
 * A332020  Positive integers m which are quadratic residues modulo prime(m).
 * https://oeis.org/A332020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, jacobi_symbol, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A332020 – integers m that are quadratic residues modulo prime(m).
   Computation: test successive m, compute Legendre symbol (m / prime(m))
   via Jacobi symbol; store m when the symbol equals +1. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* cache of sequence terms */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;   /* number of terms stored   */
static size_t seq_cap  = 0;   /* current capacity of seq_cache */

/* cache of primes: primes_arr[i] == (i+1)-th prime */
static uint64_t *primes_arr = NULL;
static size_t   primes_cnt  = 0;
static size_t   primes_lim  = 0;

/* next integer to test */
static idx_t curr_m = 1;

/* ensure that primes_arr contains at least 'needed' primes */
static void
ensure_primes_len(size_t needed)
{
    if (primes_cnt >= needed)
        return;

    idx_t limit = primes_lim ? (idx_t)primes_lim : 1000;
    while (primes_cnt < needed) {
        limit *= 2;
        uint64_t *new_arr = NULL;
        size_t   new_cnt = 0;
        /* build_primes_upto allocates a new array that must be freed later */
        build_primes_upto(limit, &new_arr, &new_cnt);
        free(primes_arr);
        primes_arr = new_arr;
        primes_cnt = new_cnt;
        primes_lim = (size_t)limit;
    }
}

/* grow the sequence cache to hold at least 'needed' elements */
static void
seq_grow(size_t needed)
{
    if (seq_cap >= needed)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;

    if (seq_cache == NULL) {
        /* seq_alloc allocates an array of seq_t of the given capacity */
        seq_cache = seq_alloc(new_cap);
    } else {
        seq_cache = seq_realloc(seq_cache, seq_cap, new_cap);
    }
    seq_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n==0 */

    seq_grow((size_t)n);

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        /* make sure the primes array has the m-th prime           */
        ensure_primes_len((size_t)curr_m);

        uint64_t p = primes_arr[(size_t)(curr_m - 1)];
        int leg = 1;                       /* 1 => residue; -1 => nonresidue */

        if (curr_m > 1) {
            /* Jacobi symbol returns 0, 1, or -1; 0 cannot occur because m < p */
            leg = jacobi_symbol((uint64_t)curr_m, p);
        }
        /* for m==1, Legendre symbol equals 1 by definition (1 is a square) */

        if (leg == 1) {
            seq_cache[seq_len++] = (seq_t)curr_m;
        }
        curr_m++;                         /* next integer to test */
    }
    return seq_cache[(size_t)(n - 1)];
}
