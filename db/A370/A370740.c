/*
 * A370740  a(1) = 1. Thereafter a(n) is the least novel k such that
 *          A007947(k*a(n-1)) is the smallest number in A002110 which is not
 *          already a term.
 * https://oeis.org/A370740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least novel k such that rad(k * a(n-1)) equals * the
 *            smallest primorial not yet a term. The function builds the *
 *            sequence incrementally, caching all previously computed terms
 * Range:     u8 n <= 10, u16 n <= 49, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -----  Required helper includes  -------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helpers from the utils directory */
#include "../utils/next_prime.h"     /* uint64_t next_prime(uint64_t start); */
#include "../utils/floor_sqrt.h"    /* seq_t floor_sqrt(seq_t x);        */

/* -----  Global storage for the sequence  ------------------------------*/
static seq_t *seq_arr = NULL;   /* a(1) stored at seq_arr[0]          */
static size_t seq_cap = 0;      /* current allocated capacity         */
static size_t seq_len = 0;      /* number of terms already known      */

/* -----  Global storage for primorials  -------------------------------*/
static seq_t *prim_arr = NULL;  /* prim_arr[1] = 2, prim_arr[2] = 6 ... */
static size_t prim_cap = 0;     /* capacity of prim_arr                */
static size_t prim_len = 0;     /* highest primorial index computed     */
static uint64_t last_prime = 1; /* prime used for the next primorial   */

/* -----  Utility: grow a dynarray  ------------------------------------*/
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        if (new_cap < need) new_cap = need;
        *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* -----  Compute rad(x) = product of distinct prime factors of x --------*/
static seq_t rad_seq(const seq_t x)
{
    if (x <= (seq_t)1) return x;          /* rad(0)=0, rad(1)=1 */
    seq_t rad = (seq_t)1;
    seq_t tmp = x;
    seq_t p = (seq_t)2;
    while (p * p <= tmp) {
        if (tmp % p == (seq_t)0) {
            rad *= p;
            while (tmp % p == (seq_t)0)
                tmp /= p;
        }
        /* get next prime >= p+1 */
        uint64_t p64 = (uint64_t)p;
        p64 = next_prime(p64);
        p = (seq_t)p64;
    }
    if (tmp > (seq_t)1)                  /* remaining factor is prime */
        rad *= tmp;
    return rad;
}

/* -----  Generate primorials up to index 'need' (inclusive) ------------*/
static void ensure_primorials(size_t need)
{
    ensure_capacity(&prim_arr, &prim_cap, need + 2);   /* (need+1) entries, ignore index 0 */

    while (prim_len < need) {                          /* grow until prim_len == need */
        if (prim_len == 0) {                           /* first primorial: 2 */
            prim_len = 1;
            prim_arr[1] = (seq_t)2;
            last_prime = 2;
        } else {
            /* next primorial = previous * next_prime */
            uint64_t next = next_prime((uint64_t)last_prime);
            last_prime = next;
            prim_len++;
            prim_arr[prim_len] = prim_arr[prim_len - 1] * (seq_t)next;
        }
    }
}

/* -----  Test whether a candidate has already appeared ---------------*/
static bool is_novel(seq_t k)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq_arr[i] == k) return false;
    return true;
}

/* -----  Main sequence function ---------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 (not used) */

    /* Compute terms up to the requested index if necessary */
    while (seq_len < (size_t)n) {
        if (seq_len == 0) {              /* initialise with a(1)=1 */
            ensure_capacity(&seq_arr, &seq_cap, 1);
            seq_arr[0] = (seq_t)1;
            seq_len = 1;
            continue;
        }

        const seq_t prev = seq_arr[seq_len - 1];

        /* Find the next primorial that has not yet appeared */
        static size_t next_prim_idx = 1;
        ensure_primorials(next_prim_idx);
        const seq_t target = prim_arr[next_prim_idx];

        /* Search for the smallest novel k with rad(k*prev) == target */
        seq_t k = (seq_t)1;
        for (;;) {
            if (is_novel(k)) {
                seq_t prod = k * prev;
                if (rad_seq(prod) == target)
                    break;
            }
            ++k;
        }

        /* Store the new term */
        ensure_capacity(&seq_arr, &seq_cap, seq_len + 1);
        seq_arr[seq_len++] = k;

        /* Update primorial index if we just used a new primorial */
        if (k == target)
            ++next_prim_idx;
    }

    return seq_arr[n - 1];
}
